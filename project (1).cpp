#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <map>
#include <ctime>
#include <algorithm>
#include <cstdlib>
using namespace std;
// User class definition
class User {
private:
string username;
string password;
int age;
float weight;
string gender;
public:
User() : username(""), password(""), age(0), weight(0.0), gender("") {}
User(string u, string p, int a, float w, string g) :
username(u), password(p), age(a), weight(w), gender(g) {}
string getUsername() const {
return username;
}
string getPassword() const {
return password;
}
void displayProfile() const {
cout << "----------------------------------\n";
cout << " Username: " << username << endl;
cout << " Age : " << age << " years\n";
cout << " Weight : " << weight << " kg\n";
cout << " Gender : " << gender << endl;
cout << "----------------------------------\n";
}
void updateProfile() {
cout << "Update your profile:\n";
cout << "Enter new age: ";
cin >> age;
while (age <= 0 || age > 120) {
cout << "Invalid. Enter a valid age: ";
cin >> age;
}
cout << "Enter new weight (kg): ";
cin >> weight;
while (weight <= 0 || weight > 500) {
cout << "Invalid. Enter valid weight: ";
cin >> weight;
}
cout << "Enter gender (Male/Female/Other): ";
cin >> gender;
cout << "Profile updated successfully.\n";
}
string toCSV() const {
stringstream ss;
ss << username << "," << password << "," << age << "," << weight << "," << gender;
return ss.str();
}
void fromCSV(const string& line) {
stringstream ss(line);
string token;
getline(ss, username, ',');
getline(ss, password, ',');
// Safe stoi (default: 0 if invalid)
getline(ss, token, ',');
try { age = token.empty() ? 0 : stoi(token); }
catch (...) { age = 0; }
// Safe stof (default: 0.0 if invalid)
getline(ss, token, ',');
try { weight = token.empty() ? 0.0f : stof(token); }
catch (...) { weight = 0.0f; }
getline(ss, gender, ',');
}
};
// User Manager to handle multiple users
class UserManager {
private:
vector<User> users;
const string filename = "users.txt";
public:
UserManager() {
loadUsers();
}
~UserManager() {
saveUsers();
}
void loadUsers() {
users.clear();
ifstream file(filename);
string line;
while (getline(file, line)) {
User u;
u.fromCSV(line);
users.push_back(u);
}
file.close();
}
void saveUsers() const {
ofstream file(filename);
for (const auto& user : users) {
file << user.toCSV() << endl;
}
file.close();
}
bool usernameExists(const string& uname) {
for (const auto& u : users) {
if (u.getUsername() == uname) return true;
}
return false;
}
void registerUser() {
string uname, pass, gender;
int age;
float weight;
cout << "Register a new user:\n";
cout << "Enter username: ";
cin >> uname;
if (usernameExists(uname)) {
cout << "Username already exists. Please try another.\n";
return;
}
cout << "Enter password: ";
cin >> pass;
cout << "Enter age: ";
cin >> age;
while (age <= 0 || age > 120) {
cout << "Invalid. Enter a valid age: ";
cin >> age;
}
cout << "Enter weight (kg): ";
cin >> weight;
while (weight <= 0 || weight > 500) {
cout << "Invalid. Enter a valid weight: ";
cin >> weight;
}
cout << "Enter gender (Male/Female/Other): ";
cin >> gender;
User newUser(uname, pass, age, weight, gender);
users.push_back(newUser);
saveUsers();
cout << "User registered successfully.\n";
}
User* loginUser() {
string uname, pass;
cout << "Enter username: ";
cin >> uname;
cout << "Enter password: ";
cin >> pass;
for (auto& u : users) {
if (u.getUsername() == uname && u.getPassword() == pass) {
cout << "Login successful. Welcome, " << uname << "!\n";
return &u;
}
}
cout << "Invalid credentials. Try again.\n";
return nullptr;
}
void userMenu(User* user) {
int choice;
do {
cout << "\n--- User Menu ---\n";
cout << "1. View Profile\n";
cout << "2. Update Profile\n";
cout << "3. Logout\n";
cout << "Choose an option: ";
cin >> choice;
switch (choice) {
case 1:
user->displayProfile();
break;
case 2:
user->updateProfile();
saveUsers();
break;
case 3:
cout << "Logging out...\n";
break;
default:
cout << "Invalid choice. Try again.\n";
}
} while (choice != 3);
}
void mainMenu() {
int choice;
do {
cout << "\n=== Fitness Tracker User System ===\n";
cout << "1. Register\n";
cout << "2. Login\n";
cout << "3. Exit\n";
cout << "Enter your choice: ";
cin >> choice;
switch (choice) {
case 1:
registerUser();
break;
case 2: {
User* u = loginUser();
if (u) userMenu(u);
break;
}
case 3:
cout << "Exiting... Goodbye!\n";
break;
default:
cout << "Invalid option. Try again.\n";
}
} while (choice != 3);
}
};
class Activity {
private:
string type;
float duration; // in minutes
float distance; // in kilometers
string date; // YYYY-MM-DD
public:
Activity() : type(""), duration(0), distance(0), date("") {}
Activity(string t, float d, float dist, string dt)
: type(t), duration(d), distance(dist), date(dt) {}
void display() const {
cout << left << setw(12) << type
<< setw(10) << duration
<< setw(10) << distance
<< setw(12) << date << endl;
}
string getType() const { return type; }
float getDuration() const { return duration; }
float getDistance() const { return distance; }
string getDate() const { return date; }
float estimateCalories() const {
float MET = 0;
if (type == "Walk") MET = 3.8;
else if (type == "Run") MET = 7.0;
else if (type == "Cycle") MET = 6.0;
else if (type == "Swim") MET = 5.8;
else if (type == "Yoga") MET = 2.5;
else MET = 3.0;
return MET * duration; // simplified: MET * time
}
string toCSV() const {
stringstream ss;
ss << type << "," << duration << "," << distance << "," << date;
return ss.str();
}
void fromCSV(const string& line) {
stringstream ss(line);
string token;
getline(ss, type, ',');
// Safe stof (default: 0.0 if invalid)
getline(ss, token, ',');
try { duration = token.empty() ? 0.0f : stof(token); }
catch (...) { duration = 0.0f; }
// Safe stof (default: 0.0 if invalid)
getline(ss, token, ',');
try { distance = token.empty() ? 0.0f : stof(token); }
catch (...) { distance = 0.0f; }
getline(ss, date, ',');
}
};
class ActivityTracker {
private:
vector<Activity> logs;
const string filename = "activity_logs.csv";
public:
ActivityTracker() {
loadFromFile();
}
~ActivityTracker() {
saveToFile();
}
void loadFromFile() {
logs.clear();
ifstream file(filename);
string line;
while (getline(file, line)) {
Activity a;
a.fromCSV(line);
logs.push_back(a);
}
file.close();
}
void saveToFile() const {
ofstream file(filename);
for (const auto& a : logs) {
file << a.toCSV() << endl;
}
file.close();
}
void addActivity() {
string type, date;
float duration, distance;
cout << "Enter activity type (Walk, Run, Cycle, Swim, Yoga): ";
cin >> type;
cout << "Enter duration in minutes: ";
cin >> duration;
while (duration <= 0) {
cout << "Invalid. Enter positive duration: ";
cin >> duration;
}
cout << "Enter distance in km (0 if not applicable): ";
cin >> distance;
while (distance < 0) {
cout << "Invalid. Enter non-negative distance: ";
cin >> distance;
}
cout << "Enter date (YYYY-MM-DD): ";
cin >> date;
Activity act(type, duration, distance, date);
logs.push_back(act);
saveToFile();
cout << "Activity logged successfully.\n";
}
void displayAllActivities() const {
if (logs.empty()) {
cout << "No activities logged.\n";
return;
}
cout << left << setw(12) << "Type"
<< setw(10) << "Duration"
<< setw(10) << "Distance"
<< setw(12) << "Date" << endl;
cout << string(44, '-') << endl;
for (const auto& a : logs)
a.display();
}
void displayCaloriesReport() const {
float total = 0;
for (const auto& a : logs) {
total += a.estimateCalories();
}
cout << "Total Calories Burned: " << fixed << setprecision(2) << total << " kcal\n";
}
void displayWeeklySummary() const {
map<string, float> weeklyCalories;
for (const auto& a : logs) {
string date = a.getDate();
weeklyCalories[date] += a.estimateCalories();
}
cout << "\n--- Weekly Calories Burned ---\n";
for (const auto& entry : weeklyCalories) {
cout << entry.first << ": " << entry.second << " kcal\n";
}
}
void filterByType(const string& type) const {
bool found = false;
cout << "\nActivities of type: " << type << "\n";
cout << left << setw(12) << "Type"
<< setw(10) << "Duration"
<< setw(10) << "Distance"
<< setw(12) << "Date" << endl;
cout << string(44, '-') << endl;
for (const auto& a : logs) {
if (a.getType() == type) {
a.display();
found = true;
}
}
if (!found) cout << "No activity of type " << type << " found.\n";
}
void sortByDuration() {
sort(logs.begin(), logs.end(), [](const Activity& a, const Activity& b) {
return a.getDuration() > b.getDuration();
});
cout << "Activities sorted by duration (descending).\n";
displayAllActivities();
}
void sortByDate() {
sort(logs.begin(), logs.end(), [](const Activity& a, const Activity& b) {
return a.getDate() < b.getDate();
});
cout << "Activities sorted by date.\n";
displayAllActivities();
}
void showMenu() {
int choice;
do {
cout << "\n==== Activity Tracker Menu ====\n";
cout << "1. Add Activity\n";
cout << "2. View All Activities\n";
cout << "3. View Calories Report\n";
cout << "4. Weekly Summary\n";
cout << "5. Filter by Type\n";
cout << "6. Sort by Duration\n";
cout << "7. Sort by Date\n";
cout << "8. Exit\n";
cout << "Enter choice: ";
cin >> choice;
switch (choice) {
case 1: addActivity(); break;
case 2: displayAllActivities(); break;
case 3: displayCaloriesReport(); break;
case 4: displayWeeklySummary(); break;
case 5: {
string t;
cout << "Enter type to filter (e.g., Walk): ";
cin >> t;
filterByType(t);
break;
}
case 6: sortByDuration(); break;
case 7: sortByDate(); break;
case 8: cout << "Exiting activity tracker...\n"; break;
default: cout << "Invalid choice.\n";
}
} while (choice != 8);
}
};
class Goal {
private:
string id;
string type; // "Steps", "Distance", "Calories", "Duration"
float target;
float progress;
string deadline; // Format: YYYY-MM-DD
bool completed;
public:
Goal() : id(""), type(""), target(0), progress(0), deadline(""), completed(false) {}
Goal(string id, string type, float target, string deadline)
: id(id), type(type), target(target), progress(0), deadline(deadline), completed(false) {}
void display() const {
cout << left << setw(10) << id
<< setw(12) << type
<< setw(10) << target
<< setw(10) << progress
<< setw(14) << deadline
<< setw(10) << (completed ? "Yes" : "No") << endl;
}
string getId() const { return id; }
string getType() const { return type; }
float getTarget() const { return target; }
float getProgress() const { return progress; }
string getDeadline() const { return deadline; }
bool isCompleted() const { return completed; }
void updateProgress(float value) {
if (completed) return;
progress += value;
if (progress >= target) {
completed = true;
progress = target;
}
}
float getProgressPercentage() const {
return (progress / target) * 100.0f;
}
string toCSV() const {
stringstream ss;
ss << id << "," << type << "," << target << "," << progress << "," << deadline << "," << completed;
return ss.str();
}
void fromCSV(const string& line) {
stringstream ss(line);
string token;
getline(ss, id, ',');
getline(ss, type, ',');
// Safe stof (default: 0.0 if invalid)
getline(ss, token, ',');
try { target = token.empty() ? 0.0f : stof(token); }
catch (...) { target = 0.0f; }
// Safe stof (default: 0.0 if invalid)
getline(ss, token, ',');
try { progress = token.empty() ? 0.0f : stof(token); }
catch (...) { progress = 0.0f; }
getline(ss, deadline, ',');
getline(ss, token, ','); completed = (token == "1" || token == "true");
}
void editGoal(float newTarget, string newDeadline) {
target = newTarget;
deadline = newDeadline;
if (progress > target) {
progress = target;
completed = true;
} else {
completed = false;
}
}
};
class GoalManager {
private:
vector<Goal> goals;
const string filename = "goal_data.csv";
string generateId() {
return "G" + to_string(goals.size() + 1);
}
public:
GoalManager() {
loadGoals();
}
~GoalManager() {
saveGoals();
}
void loadGoals() {
goals.clear();
ifstream file(filename);
string line;
while (getline(file, line)) {
Goal g;
g.fromCSV(line);
goals.push_back(g);
}
file.close();
}
void saveGoals() const {
ofstream file(filename);
for (const auto& g : goals) {
file << g.toCSV() << endl;
}
file.close();
}
void addGoal() {
string type, deadline;
float target;
cout << "Enter goal type (Steps, Distance, Duration, Calories): ";
cin >> type;
cout << "Enter target value: ";
cin >> target;
while (target <= 0) {
cout << "Target must be positive. Re-enter: ";
cin >> target;
}
cout << "Enter deadline (YYYY-MM-DD): ";
cin >> deadline;
string id = generateId();
Goal g(id, type, target, deadline);
goals.push_back(g);
saveGoals();
cout << "Goal added successfully.\n";
}
void viewGoals() const {
if (goals.empty()) {
cout << "No goals set.\n";
return;
}
cout << left << setw(10) << "ID"
<< setw(12) << "Type"
<< setw(10) << "Target"
<< setw(10) << "Progress"
<< setw(14) << "Deadline"
<< setw(10) << "Done" << endl;
cout << string(66, '-') << endl;
for (const auto& g : goals)
g.display();
}
void updateGoalProgress() {
string id;
float value;
cout << "Enter Goal ID to update: ";
cin >> id;
auto it = find_if(goals.begin(), goals.end(), [&](const Goal& g) {
return g.getId() == id;
});
if (it != goals.end()) {
cout << "Enter value to add to progress: ";
cin >> value;
if (value <= 0) {
cout << "Invalid value.\n";
return;
}
it->updateProgress(value);
saveGoals();
cout << "Progress updated.\n";
} else {
cout << "Goal ID not found.\n";
}
}
void markGoalAsCompleted() {
string id;
cout << "Enter Goal ID to mark as completed: ";
cin >> id;
auto it = find_if(goals.begin(), goals.end(), [&](const Goal& g) {
return g.getId() == id;
});
if (it != goals.end()) {
it->updateProgress(it->getTarget());
saveGoals();
cout << "Marked as completed.\n";
} else {
cout << "Goal not found.\n";
}
}
void deleteGoal() {
string id;
cout << "Enter Goal ID to delete: ";
cin >> id;
auto it = remove_if(goals.begin(), goals.end(), [&](const Goal& g) {
return g.getId() == id;
});
if (it != goals.end()) {
goals.erase(it, goals.end());
saveGoals();
cout << "Goal deleted.\n";
} else {
cout << "Goal not found.\n";
}
}
void showProgressBars() const {
cout << "\n--- Goal Progress Summary ---\n";
for (const auto& g : goals) {
cout << g.getId() << " [" << g.getType() << "] ";
float perc = g.getProgressPercentage();
int bars = static_cast<int>(perc / 5);
cout << "[";
for (int i = 0; i < 20; ++i) {
if (i < bars) cout << "#";
else cout << "-";
}
cout << "] " << fixed << setprecision(1) << perc << "%\n";
}
}
void filterByType(const string& type) const {
cout << "\nGoals of type: " << type << "\n";
bool found = false;
for (const auto& g : goals) {
if (g.getType() == type) {
g.display();
found = true;
}
}
if (!found)
cout << "No goals found for type: " << type << "\n";
}
void editGoalDetails() {
string id;
float newTarget;
string newDeadline;
cout << "Enter Goal ID to edit: ";
cin >> id;
auto it = find_if(goals.begin(), goals.end(), [&](const Goal& g) {
return g.getId() == id;
});
if (it != goals.end()) {
cout << "Enter new target: ";
cin >> newTarget;
cout << "Enter new deadline (YYYY-MM-DD): ";
cin >> newDeadline;
it->editGoal(newTarget, newDeadline);
saveGoals();
cout << "Goal updated.\n";
} else {
cout << "Goal not found.\n";
}
}
void showMenu() {
int choice;
do {
cout << "\n==== Goal Manager Menu ====\n";
cout << "1. Add New Goal\n";
cout << "2. View All Goals\n";
cout << "3. Update Goal Progress\n";
cout << "4. Mark Goal as Completed\n";
cout << "5. Delete Goal\n";
cout << "6. Show Progress Bars\n";
cout << "7. Filter by Type\n";
cout << "8. Edit Goal Details\n";
cout << "9. Exit\n";
cout << "Enter your choice: ";
cin >> choice;
switch (choice) {
case 1: addGoal(); break;
case 2: viewGoals(); break;
case 3: updateGoalProgress(); break;
case 4: markGoalAsCompleted(); break;
case 5: deleteGoal(); break;
case 6: showProgressBars(); break;
case 7: {
string type;
cout << "Enter goal type to filter: ";
cin >> type;
filterByType(type);
break;
}
case 8: editGoalDetails(); break;
case 9: cout << "Exiting Goal Manager...\n"; break;
default: cout << "Invalid option. Try again.\n";
}
} while (choice != 9);
}
};
struct ProgressEntry {
string date; // YYYY-MM-DD
string activity; // Steps, Distance, Calories, Duration
float value;
};
class ProgressTracker {
private:
vector<ProgressEntry> entries;
const string filename = "progress_data.csv";
string getTodayDate() {
time_t now = time(0);
tm *ltm = localtime(&now);
char dateStr[11];
strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", ltm);
return string(dateStr);
}
void loadFromFile() {
entries.clear();
ifstream file(filename);
string line;
while (getline(file, line)) {
stringstream ss(line);
string date, activity, valStr;
float val = 0.0f; // Default if conversion fails
getline(ss, date, ',');
getline(ss, activity, ',');
getline(ss, valStr, ',');
// Safe stof (default: 0.0 if invalid)
try { val = valStr.empty() ? 0.0f : stof(valStr); }
catch (...) { val = 0.0f; }
entries.push_back({date, activity, val});
}
file.close();
}
void saveToFile() const {
ofstream file(filename);
for (const auto &entry : entries) {
file << entry.date << "," << entry.activity << "," << entry.value << endl;
}
file.close();
}
public:
ProgressTracker() {
loadFromFile();
}
void logActivity(const string &activityType, float value, const string &date = "") {
string useDate = date.empty() ? getTodayDate() : date;
entries.push_back({useDate, activityType, value});
saveToFile();
cout << "Logged " << value << " of " << activityType << " on " << useDate << endl;
}
void viewDailySummary(const string &date = "") {
string targetDate = date.empty() ? getTodayDate() : date;
map<string, float> dailyData;
for (const auto &e : entries) {
if (e.date == targetDate) {
dailyData[e.activity] += e.value;
}
}
if (dailyData.empty()) {
cout << "No data for " << targetDate << endl;
return;
}
cout << "\n--- Daily Summary for " << targetDate << " ---\n";
for (auto it = dailyData.begin(); it != dailyData.end(); ++it) {
    const string& activity = it->first;
    int value = it->second;
    cout << setw(10) << left << activity << ": " << value << endl;
}
}
void viewWeeklySummary() {
time_t now = time(0);
tm *ltm = localtime(&now);
char today[11];
strftime(today, sizeof(today), "%Y-%m-%d", ltm);
map<string, float> summary;
for (int i = 0; i < 7; ++i) {
time_t temp = now - i * 86400;
tm *day = localtime(&temp);
char dateBuf[11];
strftime(dateBuf, sizeof(dateBuf), "%Y-%m-%d", day);
string currDate(dateBuf);
for (const auto &e : entries) {
if (e.date == currDate) {
summary[e.activity] += e.value;
}
}
}
cout << "\n--- Weekly Summary (Last 7 Days) ---\n";
for (const auto& pair : summary) {
    const auto& activity = pair.first;
    const auto& total = pair.second;
    cout << setw(10) << left << activity << ": " << total << endl;
}

}
void viewAllEntries() const {
if (entries.empty()) {
cout << "No progress data available.\n";
return;
}
cout << "\n--- All Logged Progress ---\n";
cout << left << setw(12) << "Date"
<< setw(12) << "Activity"
<< setw(8) << "Value" << endl;
cout << string(32, '-') << endl;
for (const auto &e : entries) {
cout << left << setw(12) << e.date
<< setw(12) << e.activity
<< setw(8) << e.value << endl;
}
}
void searchByActivity(const string &activityType) const {
bool found = false;
cout << "\n--- Entries for Activity: " << activityType << " ---\n";
for (const auto &e : entries) {
if (e.activity == activityType) {
cout << setw(12) << e.date << setw(8) << e.value << endl;
found = true;
}
}
if (!found) {
cout << "No entries found for this activity.\n";
}
}
void removeEntryByDateAndActivity(const string &date, const string &activityType) {
auto it = remove_if(entries.begin(), entries.end(), [&](const ProgressEntry &e) {
return e.date == date && e.activity == activityType;
});
if (it != entries.end()) {
entries.erase(it, entries.end());
saveToFile();
cout << "Entry removed.\n";
} else {
cout << "No matching entry found.\n";
}
}
void editEntry(const string &date, const string &activityType, float newValue) {
bool updated = false;
for (auto &e : entries) {
if (e.date == date && e.activity == activityType) {
e.value = newValue;
updated = true;
}
}
if (updated) {
saveToFile();
cout << "Entry updated successfully.\n";
} else {
cout << "No matching entry found.\n";
}
}
void showMenu() {
int choice;
do {
cout << "\n===== Progress Tracker Menu =====\n";
cout << "1. Log Activity\n";
cout << "2. View Daily Summary\n";
cout << "3. View Weekly Summary\n";
cout << "4. View All Entries\n";
cout << "5. Search By Activity\n";
cout << "6. Remove Entry\n";
cout << "7. Edit Entry\n";
cout << "8. Exit\n";
cout << "Enter your choice: ";
cin >> choice;
string activity, date;
float value;
switch (choice) {
case 1:
cout << "Enter activity (Steps/Distance/Calories/Duration): ";
cin >> activity;
cout << "Enter value: ";
cin >> value;
logActivity(activity, value);
break;
case 2:
cout << "Enter date (YYYY-MM-DD) or leave blank for today: ";
cin.ignore();
getline(cin, date);
viewDailySummary(date);
break;
case 3:
viewWeeklySummary();
break;
case 4:
viewAllEntries();
break;
case 5:
cout << "Enter activity type to search: ";
cin >> activity;
searchByActivity(activity);
break;
case 6:
cout << "Enter date (YYYY-MM-DD): ";
cin >> date;
cout << "Enter activity: ";
cin >> activity;
removeEntryByDateAndActivity(date, activity);
break;
case 7:
cout << "Enter date (YYYY-MM-DD): ";
cin >> date;
cout << "Enter activity: ";
cin >> activity;
cout << "Enter new value: ";
cin >> value;
editEntry(date, activity, value);
break;
case 8:
cout << "Exiting Progress Tracker...\n";
break;
default:
cout << "Invalid choice. Try again.\n";
}
} while (choice != 8);
}
};
class DataManager {
private:
string userFile = "user_data.csv"; // File for user information
string activityFile = "activity_data.csv"; // File for activity data
string goalFile = "goal_data.csv"; // File for goal data
string progressFile = "progress_data.csv"; // File for progress data
// Helper function to load data from CSV file into vector of strings
vector<string> loadDataFromFile(const string &filename) {
ifstream file(filename);
vector<string> data;
string line;
while (getline(file, line)) {
data.push_back(line);
}
file.close();
return data;
}
// Helper function to save vector of strings to CSV file
void saveDataToFile(const string &filename, const vector<string> &data) {
ofstream file(filename);
for (const auto &line : data) {
file << line << endl;
}
file.close();
}
public:
// Load user data
map<string, string> loadUserData() {
map<string, string> userData;
vector<string> data = loadDataFromFile(userFile);
for (const auto &line : data) {
stringstream ss(line);
string key, value;
getline(ss, key, ',');
getline(ss, value, ',');
userData[key] = value;
}
return userData;
}
// Save user data
void saveUserData(const map<string, string> &userData) {
vector<string> data;
for (const auto& pair : userData) {
    const auto& key = pair.first;
    const auto& value = pair.second;
    data.push_back(key + "," + value);
}
saveDataToFile(userFile, data);
}
// Load activity data
vector<string> loadActivityData() {
return loadDataFromFile(activityFile);
}
// Save activity data
void saveActivityData(const vector<string> &activityData) {
saveDataToFile(activityFile, activityData);
}
// Load goal data
vector<string> loadGoalData() {
return loadDataFromFile(goalFile);
}
// Save goal data
void saveGoalData(const vector<string> &goalData) {
saveDataToFile(goalFile, goalData);
}
// Load progress data
vector<string> loadProgressData() {
return loadDataFromFile(progressFile);
}
// Save progress data
void saveProgressData(const vector<string> &progressData) {
saveDataToFile(progressFile, progressData);
}
// Display user data for verification
void displayUserData() {
map<string, string> userData = loadUserData();
cout << "\n--- User Data ---\n";
for (const auto& pair : userData) {
    const auto& key = pair.first;
    const auto& value = pair.second;
    cout << key << ": " << value << endl;
}
}
// Display activity data for verification
void displayActivityData() {
vector<string> activityData = loadActivityData();
cout << "\n--- Activity Data ---\n";
for (const auto &line : activityData) {
cout << line << endl;
}
}
// Display goal data for verification
void displayGoalData() {
vector<string> goalData = loadGoalData();
cout << "\n--- Goal Data ---\n";
for (const auto &line : goalData) {
cout << line << endl;
}
}
// Display progress data for verification
void displayProgressData() {
vector<string> progressData = loadProgressData();
cout << "\n--- Progress Data ---\n";
for (const auto &line : progressData) {
cout << line << endl;
}
}
// Update a specific user data field
void updateUserField(const string &key, const string &newValue) {
map<string, string> userData = loadUserData();
if (userData.find(key) != userData.end()) {
userData[key] = newValue;
saveUserData(userData);
cout << key << " updated to " << newValue << endl;
} else {
cout << key << " not found in user data.\n";
}
}
// Update a specific activity data entry
void updateActivityData(int index, const string &newData) {
vector<string> activityData = loadActivityData();
if (index >= 0 && index < activityData.size()) {
activityData[index] = newData;
saveActivityData(activityData);
cout << "Activity data at index " << index << " updated.\n";
} else {
cout << "Invalid index.\n";
}
}
// Update a specific goal data entry
void updateGoalData(int index, const string &newGoal) {
vector<string> goalData = loadGoalData();
if (index >= 0 && index < goalData.size()) {
goalData[index] = newGoal;
saveGoalData(goalData);
cout << "Goal data at index " << index << " updated.\n";
} else {
cout << "Invalid index.\n";
}
}
// Update a specific progress data entry
void updateProgressData(int index, const string &newProgress) {
vector<string> progressData = loadProgressData();
if (index >= 0 && index < progressData.size()) {
progressData[index] = newProgress;
saveProgressData(progressData);
cout << "Progress data at index " << index << " updated.\n";
} else {
cout << "Invalid index.\n";
}
}
};
class ReportAnalyzer {
private:
float heightCm;
float weightKg;
float averageSteps;
float averageCalories;
float averageDuration;
float calculateBMI() const {
float heightM = heightCm / 100.0f;
return weightKg / (heightM * heightM);
}
string interpretBMI(float bmi) const {
if (bmi < 18.5) return "Underweight";
else if (bmi < 24.9) return "Normal weight";
else if (bmi < 29.9) return "Overweight";
else return "Obese";
}
void suggestBasedOnBMI(float bmi) const {
cout << "\n--- BMI-Based Suggestion ---\n";
if (bmi < 18.5)
cout << "You are underweight. Consider a calorie-rich balanced diet and light strength training.\n";
else if (bmi < 24.9)
cout << "You have a normal weight. Maintain your lifestyle with regular exercise and healthy meals.\n";
else if (bmi < 29.9)
cout << "You are overweight. Try to include cardio exercises and reduce sugar intake.\n";
else
cout << "You are obese. Consult a healthcare provider and begin with light exercise and diet control.\n";
}
void suggestBasedOnActivity() const {
cout << "\n--- Activity Suggestions ---\n";
if (averageSteps < 5000)
cout << "Try to walk more. Aim for at least 7,000–10,000 steps daily.\n";
else if (averageSteps < 8000)
cout << "Good job! Slight improvement can get you to optimal levels.\n";
else
cout << "Great step count! Keep up the active lifestyle.\n";
if (averageDuration < 30)
cout << "Try to be active for at least 30 minutes a day.\n";
else
cout << "You're meeting the daily activity duration goal!\n";
}
void suggestBasedOnCalories() const {
cout << "\n--- Calorie Burn Suggestions ---\n";
if (averageCalories < 200)
cout << "Increase your activity to burn at least 300–500 calories daily.\n";
else if (averageCalories < 400)
cout << "You're doing okay. Consider light jogging or interval workouts.\n";
else
cout << "Good calorie burn rate. Maintain with consistent workouts.\n";
}
public:
ReportAnalyzer(float height, float weight,
float avgSteps, float avgCalories, float avgDuration)
: heightCm(height), weightKg(weight),
averageSteps(avgSteps), averageCalories(avgCalories),
averageDuration(avgDuration) {}
void generateReport() {
cout << fixed << setprecision(2);
cout << "\n===== Health Report =====\n";
float bmi = calculateBMI();
string bmiStatus = interpretBMI(bmi);
cout << "Height: " << heightCm << " cm\n";
cout << "Weight: " << weightKg << " kg\n";
cout << "BMI: " << bmi << " (" << bmiStatus << ")\n";
cout << "Average Steps/Day: " << averageSteps << endl;
cout << "Average Calories Burned/Day: " << averageCalories << endl;
cout << "Average Activity Duration/Day: " << averageDuration << " mins\n";
suggestBasedOnBMI(bmi);
suggestBasedOnActivity();
suggestBasedOnCalories();
}
};
int main() {
UserManager um;
ActivityTracker tracker;
GoalManager gm;
ProgressTracker ptracker;
DataManager dataManager;
int choice;
do {
cout << "\n======= FITNESS TRACKER MAIN MENU =======\n";
cout << "1. Manage User Profile\n";
cout << "2. Activity Tracker\n";
cout << "3. Goal Manager\n";
cout << "4. Progress Tracker\n";
cout << "5. View All Stored Data\n";
cout << "6. Update Stored Data\n";
cout << "7. Generate Health Report & Suggestions\n";
cout << "0. Exit\n";
cout << "Enter your choice: ";
cin >> choice;
switch (choice) {
case 1:
um.mainMenu();
break;
case 2:
tracker.showMenu();
break;
case 3:
gm.showMenu();
break;
case 4:
ptracker.showMenu();
break;
case 5:
cout << "\n--- Displaying All Data ---\n";
dataManager.displayUserData();
dataManager.displayActivityData();
dataManager.displayGoalData();
dataManager.displayProgressData();
break;
case 6: {
cout << "\n--- Update Data ---\n";
int updateChoice;
cout << "1. Update User Field\n";
cout << "2. Update Activity Entry\n";
cout << "3. Update Goal Entry\n";
cout << "4. Update Progress Entry\n";
cout << "Enter your choice: ";
cin >> updateChoice;
cin.ignore(); // flush newline
switch (updateChoice) {
case 1: {
string key, newValue;
cout << "Enter field to update (e.g., Age): ";
getline(cin, key);
cout << "Enter new value: ";
getline(cin, newValue);
dataManager.updateUserField(key, newValue);
break;
}
case 2: {
int index;
string newData;
cout << "Enter activity index to update: ";
cin >> index;
cin.ignore();
cout << "Enter new activity data: ";
getline(cin, newData);
dataManager.updateActivityData(index, newData);
break;
}
case 3: {
int index;
string newGoal;
cout << "Enter goal index to update: ";
cin >> index;
cin.ignore();
cout << "Enter new goal: ";
getline(cin, newGoal);
dataManager.updateGoalData(index, newGoal);
break;
}
case 4: {
int index;
string newProgress;
cout << "Enter progress index to update: ";
cin >> index;
cin.ignore();
cout << "Enter new progress: ";
getline(cin, newProgress);
dataManager.updateProgressData(index, newProgress);
break;
}
default:
cout << "Invalid update choice.\n";
break;
}
break;
}
case 7: {
float height, weight;
float avgSteps, avgCalories, avgDuration;
cout << "\n--- Health Report & Suggestions ---\n";
cout << "Enter your height (in cm): ";
cin >> height;
cout << "Enter your weight (in kg): ";
cin >> weight;
cout << "Enter average daily steps: ";
cin >> avgSteps;
cout << "Enter average daily calories burned: ";
cin >> avgCalories;
cout << "Enter average daily activity duration (in minutes): ";
cin >> avgDuration;
ReportAnalyzer analyzer(height, weight, avgSteps, avgCalories, avgDuration);
analyzer.generateReport();
break;
}
case 0:
cout << "Exiting program. Stay fit!\n";
break;
default:
cout << "Invalid option. Try again.\n";
break;
}
} while (choice != 0);
return 0;
}
