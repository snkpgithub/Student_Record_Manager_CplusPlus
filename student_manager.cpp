
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <limits>

using namespace std;

class Student {
public:
    int roll;
    string name;
    float marks;

    void input() {
        roll = getIntInput("Enter Roll Number: ");
        cout << "Enter Name: ";
        cin.ignore(); // clear newline from previous input
        getline(cin, name);
        marks = getFloatInput("Enter Marks: ");
    }

    void display() const {
        cout << "Roll: " << roll << ", Name: " << name << ", Marks: " << marks << endl;
    }

    string serialize() const {
        return to_string(roll) + "," + name + "," + to_string(marks);
    }

    static Student deserialize(const string& line) {
        Student s;
        size_t pos1 = line.find(",");
        size_t pos2 = line.rfind(",");

        s.roll = stoi(line.substr(0, pos1));
        s.name = line.substr(pos1 + 1, pos2 - pos1 - 1);
        s.marks = stof(line.substr(pos2 + 1));

        return s;
    }

private:
    int getIntInput(const string& prompt) {
        int value;
        while (true) {
            cout << prompt;
            cin >> value;
            if (!cin.fail()) break;
            showError();
        }
        return value;
    }

    float getFloatInput(const string& prompt) {
        float value;
        while (true) {
            cout << prompt;
            cin >> value;
            if (!cin.fail()) break;
            showError();
        }
        return value;
    }

    void showError() {
        cout << "❌ Enter a valid input!" << endl;
        cin.clear(); // clear error flag
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // discard invalid input
    }
};

void saveToFile(const vector<Student>& students) {
    ofstream out("records.txt");
    for (const auto& s : students) {
        out << s.serialize() << endl;
    }
    out.close();
}

vector<Student> loadFromFile() {
    vector<Student> students;
    ifstream in("records.txt");
    string line;
    while (getline(in, line)) {
        students.push_back(Student::deserialize(line));
    }
    in.close();
    return students;
}

int getValidatedMenuChoice() {
    int choice;
    while (true) {
        cout << "Choose: ";
        cin >> choice;
        if (!cin.fail()) break;
        cout << "❌ Invalid menu choice. Please enter a number.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return choice;
}

int main() {
    vector<Student> students = loadFromFile();
    int choice;

    do {
        cout << "\n--- Student Record Manager ---\n";
        cout << "1. Add Student\n2. Display All\n3. Search by Roll\n4. Save & Exit\n";
        choice = getValidatedMenuChoice();

        if (choice == 1) {
            Student s;
            s.input();
            students.push_back(s);
        } else if (choice == 2) {
            for (const auto& s : students) {
                s.display();
            }
        } else if (choice == 3) {
            int roll;
            cout << "Enter roll to search: ";
            cin >> roll;
            if (cin.fail()) {
                cout << "❌ Invalid roll number input!\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            bool found = false;
            for (const auto& s : students) {
                if (s.roll == roll) {
                    s.display();
                    found = true;
                    break;
                }
            }
            if (!found) cout << "Student not found!\n";
        } else if (choice == 4) {
            saveToFile(students);
            cout << "✅ Data saved. Exiting...\n";
        } else {
            cout << "❌ Invalid option. Try again.\n";
        }

    } while (choice != 4);

    return 0;
}
