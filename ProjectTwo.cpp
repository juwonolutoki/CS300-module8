#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>

// Course class definition
class Course {
public:
    std::string code;
    std::string name;
    std::vector<std::string> prerequisites;

    // Constructor
    Course(const std::string& c, const std::string& n) : code(c), name(n) {}

    // Function to add prerequisite
    void addPrerequisite(const std::string& prereq) {
        prerequisites.push_back(prereq);
    }

    // Function to display course details including prerequisites
    void display() const {
        std::cout << "Course Code: " << code << "\n";
        std::cout << "Course Name: " << name << "\n";
        if (!prerequisites.empty()) {
            std::cout << "Prerequisites:\n";
            for (const auto& prereq : prerequisites) {
                std::cout << "- " << prereq << "\n";
            }
        }
        std::cout << "\n";
    }

    // Function to compare courses for sorting (alphanumeric by code)
    bool operator<(const Course& other) const {
        return code < other.code;
    }
};

// Function to read course data from file
std::vector<Course> readCourseDataFromFile(const std::string& filename) {
    std::vector<Course> courses;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << "\n";
        return courses;
    }

    std::string code, name, prereq;
    std::map<std::string, Course> courseMap;

    while (file >> code >> name) {
        if (!courseMap.count(code)) {
            courseMap[code] = Course(code, name);
        }
        else {
            courseMap[code].name = name; // Update name if code already exists
        }

        std::getline(file, prereq);
        if (prereq.find('\t') != std::string::npos) {
            std::istringstream iss(prereq.substr(1)); // skip initial tab
            while (iss >> prereq) {
                courseMap[code].addPrerequisite(prereq);
            }
        }
    }

    for (const auto& pair : courseMap) {
        courses.push_back(pair.second);
    }

    file.close();
    return courses;
}

// Function to print sorted list of courses by course code
void printSortedCourses(const std::vector<Course>& courses) {
    std::vector<Course> sortedCourses = courses;
    std::sort(sortedCourses.begin(), sortedCourses.end());

    std::cout << "Alphanumeric list of courses:\n";
    for (const auto& course : sortedCourses) {
        course.display();
    }
}

// Function to find and display course details by course code
void displayCourseDetails(const std::vector<Course>& courses, const std::string& courseCode) {
    bool found = false;
    for (const auto& course : courses) {
        if (course.code == courseCode) {
            course.display();
            found = true;
            break;
        }
    }
    if (!found) {
        std::cout << "Course with code " << courseCode << " not found.\n\n";
    }
}

// Function to display the menu and handle user input
void displayMenu() {
    std::cout << "\nMenu:\n";
    std::cout << "1. Load file data into data structure\n";
    std::cout << "2. Print an alphanumeric list of all courses\n";
    std::cout << "3. Print course title and prerequisites for any individual course\n";
    std::cout << "9. Exit the program\n";
    std::cout << "Enter your choice: ";
}

int main() {
    std::vector<Course> courses;
    bool dataLoaded = false;

    while (true) {
        displayMenu();

        int choice;
        std::cin >> choice;
        std::cin.ignore(); // Clear newline from input buffer

        switch (choice) {
        case 1: {
            std::string filename;
            std::cout << "Enter the file name containing course data: ";
            std::getline(std::cin, filename);
            courses = readCourseDataFromFile(filename);
            dataLoaded = true;
            break;
        }
        case 2: {
            if (!dataLoaded) {
                std::cout << "Please load data first (Option 1).\n\n";
                break;
            }
            printSortedCourses(courses);
            break;
        }
        case 3: {
            if (!dataLoaded) {
                std::cout << "Please load data first (Option 1).\n\n";
                break;
            }
            std::string courseCode;
            std::cout << "Enter the course code: ";
            std::cin >> courseCode;
            displayCourseDetails(courses, courseCode);
            break;
        }
        case 9: {
            std::cout << "Exiting the program.\n";
            return 0;
        }
        default:
            std::cout << "Invalid choice. Please enter a valid option.\n";
        }
    }

    return 0;
}
