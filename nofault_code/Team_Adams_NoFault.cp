#ifndef STUDENT_HPP
#define STUDENT_HPP

#include <iostream>
#include <string>
#include <regex>

namespace Grading{
    const unsigned int MAX_GRADE = 4;
    const unsigned int MIN_GRADE = 0;

    typedef unsigned int Grade;
    typedef char LetterGrade;

    /**
     * @brief function to validate the student's grade
     * 
     * @param g grade to validate
     * 
     * @return bool 
     */
    bool isValidGrade(Grade g){
        return g >= MIN_GRADE && g <= MAX_GRADE;
    }

    /**
     * @brief function to get the letter grade from the grade
     * 
     * @param g grade to get the letter grade from
     * 
     * @return Grading::LetterGrade 
     */
    char getLetterGrade(Grade g){
        if(g == 0){
            return 'F';
        }else if(g == 1){
            return 'D';
        }else if(g == 2){
            return 'C';
        }else if(g == 3){
            return 'B';
        }else if(g == 4){
            return 'A';
        }else{
            return 'X';
        }
    }

    /**
     * @brief function to get the grade from the letter grade
     * 
     * @param g letter grade to get the grade from
     * 
     * @return Grading::Grade 
     */
    Grade getGrade(LetterGrade g){
        if(g == 'F'){
            return 0;
        }else if(g == 'D'){
            return 1;
        }else if(g == 'C'){
            return 2;
        }else if(g == 'B'){
            return 3;
        }else if(g == 'A'){
            return MAX_GRADE;
        }else{
            return 5;
        }
    }
    
}

namespace Validation{
    const unsigned int ALPHABET_SIZE = 26;
    const char ALPHABET_LOWER_START = 'a';
    char offset = ALPHABET_LOWER_START;

    const unsigned int EMAIL_CHAR_LIMIT = 40;
    const unsigned int NAME_CHAR_LIMIT = 40;
    
    const std::string ID_PREFIX = "U";
    const unsigned int ID_CHAR_LIMIT = ID_PREFIX.length() + 9;

    /**
     * @brief function to validate the email
     * 
     * @param email email to validate
     * 
     * @return bool 
     */
    bool isValidEmail(std::string email){
        if(email.length() <= EMAIL_CHAR_LIMIT){
            return std::regex_match(email, std::regex("([a-zA-Z0-9_\\-\\.]+)@([a-zA-Z0-9_\\-\\.]+)\\.([a-zA-Z]{2,5})"));
        }
        return false;
    }

    /**
     * @brief function to validate the student's grade
     * 
     * @param g student's grade to validate
     * 
     * @return bool 
     */
    bool isValidGrade(Grading::Grade g){
        return Grading::isValidGrade(g);
    }

    /**
     * @brief function to validate the student's id
     * 
     * @param id student's id to validate
     * 
     * @return bool 
     */
    bool isValidId(std::string id){
        if(id.length() <= ID_CHAR_LIMIT){
            return std::regex_match(id, std::regex(ID_PREFIX + "[0-9]{9}")); //Prefix followed by 9 digits
        }
        return false;
    }

    /**
     * @brief function to validate the student's name
     * 
     * @param name student's name to validate
     * 
     * @return bool 
     */
    bool isValidName(std::string name){
        if(name.length() <= NAME_CHAR_LIMIT){
            return std::regex_match(name, std::regex("[a-zA-Z ]+")); //only letters and spaces
        }
        
        return false;
    }

    /**
     * @brief Generates a unique ID , based on the index passed in
     * 
     * @return std::string 
     */
    std::string generateId(unsigned int index){
        std::string temp = std::to_string(index);
        int len = ID_CHAR_LIMIT - ID_PREFIX.length() - temp.length();
        return ID_PREFIX + std::string(len, '0') + temp;
    }

     /**
     * @brief Generates a unique email, based on the index passed in
     * 
     * @return std::string 
     */
    std::string generateEmail(unsigned int index){
        return "student"+ std::to_string(index) + "@example.com";
    }

    /**
     * @brief Generates a unique name, based on the index passed in
     * 
     * @return std::string 
     */
    std::string generateName(unsigned int index){
        if(index % (ALPHABET_SIZE * NAME_CHAR_LIMIT) == 0){
            Validation::offset ^= 32; //flip the offset
        }
        int len = ((index + ALPHABET_SIZE)/ ALPHABET_SIZE) % NAME_CHAR_LIMIT;
        int alphabetChar = index % ALPHABET_SIZE + offset;
        return std::string(len == 0 ? 1 : len, alphabetChar);
    }

}

namespace StudentExceptions{
    class InvalidStudentException : public std::exception{
        private:
            std::string msg;
        public:
            InvalidStudentException(std::string msg = "Invalid Student"){
                this->msg = msg;
            }
            virtual const char* what() const throw(){
                return msg.c_str();
            }
    };

    class InvalidEmailException : public InvalidStudentException{
        public:
            InvalidEmailException(std::string msg = "Invalid Email"): InvalidStudentException(msg){}
    };

    class InvalidIdException : public InvalidStudentException{
        public:
            InvalidIdException(std::string msg = "Invalid ID"): InvalidStudentException(msg){}
    };

    class InvalidNameException : public InvalidStudentException{
        public:
            InvalidNameException(std::string msg = "Invalid Name"): InvalidStudentException(msg){}
    };

    class InvalidGradeException : public InvalidStudentException{
        public:
            InvalidGradeException(std::string msg = "Invalid Grade"): InvalidStudentException(msg){}
    };
}

class Student{
    private:
        std::string name;
        std::string id;
        std::string email;
        Grading::Grade presentationGrade;
        Grading::Grade essayGrade;
        Grading::Grade termProjectGrade;

        static unsigned int studentCount;

        /**
         * @brief Generates a unique ID for a student, based on the number of students
         * 
         * @return std::string 
         */
        static std::string generateId(){
            return Validation::generateId(studentCount);
        }

        /**
         * @brief Generates a unique email for a student, based on the number of students
         * 
         * @return std::string 
         */
        static std::string generateEmail(){
            return Validation::generateEmail(studentCount);
        }

        /**
         * @brief Generates a unique name for a student, based on the number of students
         * 
         * @return std::string 
         */
        static std::string generateName(){
            return Validation::generateName(studentCount);
        }
        

    public:
        /**
         * @brief Construct a new Student:: Student object
         * 
         */
        Student(){
            this->setId(this->generateId());
            this->setName(this->generateName());
            this->setEmail(this->generateEmail());
            this->setPresentationGrade(Grading::MAX_GRADE);
            this->setEssayGrade(Grading::MAX_GRADE);
            this->setTermProjectGrade(Grading::MAX_GRADE);
            studentCount++;
        }

        /**
         * @brief Construct a new Student:: Student object
         * 
         * @param name 
         * @param id 
         */
        Student(std::string name, std::string id){
            this->setId(id);
            this->setName(name);
            this->setEmail(this->generateEmail());
            this->setPresentationGrade(Grading::MAX_GRADE);
            this->setEssayGrade(Grading::MAX_GRADE);
            this->setTermProjectGrade(Grading::MAX_GRADE);
            studentCount++;
        }

        /**
         * @brief Copy constructor
         * 
         * @param s 
         */
        Student(const Student &s){ //Copy Constructor //Validation check isn't needed if the object is already valid
            this->setId(s.id);
            this->setName(s.name);
            this->setEmail(s.email);
            this->setPresentationGrade(s.presentationGrade);
            this->setEssayGrade(s.essayGrade);
            this->setTermProjectGrade(s.termProjectGrade);
            studentCount++;
        }

        /**
         * @brief Construct a new Student:: Student object
         * 
         * @param name 
         * @param id 
         * @param email 
         */
        Student(std::string name, std::string id, std::string email){
            this->setId(id);
            this->setName(name);
            this->setEmail(email);
            this->setPresentationGrade(Grading::MAX_GRADE);
            this->setEssayGrade(Grading::MAX_GRADE);
            this->setTermProjectGrade(Grading::MAX_GRADE);
            studentCount++;
        }

        /**
         * @brief Construct a new Student:: Student object
         * 
         * @param name 
         * @param id 
         * @param email 
         * @param presentationGrade 
         * @param essayGrade 
         * @param termProjectGrade 
         */
        Student(std::string name, std::string id, std::string email, unsigned int presentationGrade, unsigned int essayGrade, unsigned termProjectGrade){
            this->setId(id);
            this->setName(name);
            this->setEmail(email);
            this->setPresentationGrade(presentationGrade);
            this->setEssayGrade(essayGrade);
            this->setTermProjectGrade(termProjectGrade);
            studentCount++;
        }

        /**
         * @brief set the student's name, if it is valid, otherwise throw an exception
         * 
         * @param name
         */
        void setName(std::string name){
            if(Validation::isValidName(name)){
                this->name = name;
            }
            else{
                throw StudentExceptions::InvalidNameException("Invalid Name: " + name);
            }

        }

        /**
         * @brief set the student's id, if it is valid, otherwise throw an exception
         * 
         * @param id 
         */
        void setId(std::string id){
            if(Validation::isValidId(id)){
                this->id = id;
            }
            else{
                throw StudentExceptions::InvalidIdException("Invalid ID: " + id);
            }
        }

        /**
         * @brief set the student's email, if it is valid, otherwise throw an exception
         * 
         * @param email 
         */
        void setEmail(std::string email){
            if(Validation::isValidEmail(email)){
                this->email = email;
            }
            else{
                throw StudentExceptions::InvalidEmailException("Invalid Email: " + email);
            }
        }

        /**
         * @brief set the student's presentation grade, if it is valid, otherwise throw an exception
         * 
         * @param presentationGrade 
         */
        void setPresentationGrade(Grading::Grade grade){
            if(Validation::isValidGrade(grade)){
                this->presentationGrade = grade;
            }
            else{
                throw StudentExceptions::InvalidGradeException("Invalid Presentation Grade: " + std::to_string(grade));
            }
        }

        /**
         * @brief set the student's essay grade, if it is valid, otherwise throw an exception
         * 
         * @param essayGrade 
         */
        void setEssayGrade(Grading::Grade grade){
            if(Validation::isValidGrade(grade)){
                this->essayGrade = grade;
            }
            else{
                throw StudentExceptions::InvalidGradeException("Invalid Essay Grade: " + std::to_string(grade));
            }
        }

        /**
         * @brief set the student's term project grade, if it is valid, otherwise throw an exception
         * 
         * @param termProjectGrade 
         */
        void setTermProjectGrade(Grading::Grade grade){
            if(Validation::isValidGrade(grade)){
                this->termProjectGrade = grade;
            }
            else{
                throw StudentExceptions::InvalidGradeException("Invalid Term Project Grade: " + std::to_string(grade));
            }
        }

        /**
         * @brief get number of student object created so far
         * 
         * @return unsigned int
         */
        static unsigned int getStudentCount(){
            return studentCount;
        }

        /**
         * @brief Get the student's name
         * 
         * @return std::string 
         */
        std::string getId(){
            return this->id;
        }

        /**
         * @brief Get the student's name
         * 
         * @return std::string 
         */
        std::string getName(){
            return this->name;
        }

        /**
         * @brief Get the student's email
         * 
         * @return std::string 
         */
        std::string getEmail(){
            return this->email;
        }

        /**
         * @brief Get the student's presentation grade
         * 
         * @return Grading::LetterGrade 
         */
        Grading::LetterGrade getPresentationGrade(){
            return Grading::getLetterGrade(this->presentationGrade);
        }

        /**
         * @brief Get the student's essay grade
         * 
         * @return Grading::LetterGrade
         */
        Grading::LetterGrade getEssayGrade(){
            return Grading::getLetterGrade(this->essayGrade);
        }

        /**
         * @brief Get the student's term project grade
         * 
         * @return Grading::LetterGrade 
         */
        Grading::LetterGrade getTermProjectGrade(){
            return Grading::getLetterGrade(this->termProjectGrade);
        }

        /**
         * @brief get the student's id, const version
         * 
         * @return std::string 
         */
        const std::string getId() const{
            return this->id;
        }

        /**
         * @brief get the student's name, const version
         * 
         * @return std::string 
         */
        const std::string getName() const{
            return this->name;
        }

        /**
         * @brief get the student's email, const version
         * 
         * @return std::string 
         */
        const std::string getEmail() const{
            return this->email;
        }

        /**
         * @brief get the student's presentation grade, const version
         * 
         * @return Grading::LetterGrade 
         */
        const Grading::LetterGrade getPresentationGrade() const{
            return Grading::getLetterGrade(this->presentationGrade);
        }

        /**
         * @brief get the student's essay grade, const version
         * 
         * @return Grading::LetterGrade 
         */
        const Grading::LetterGrade getEssayGrade() const{
            return Grading::getLetterGrade(this->essayGrade);
        }

        /**
         * @brief get the student's term project grade, const version
         * 
         * @return Grading::LetterGrade 
         */
        const Grading::LetterGrade getTermProjectGrade() const{
            return Grading::getLetterGrade(this->termProjectGrade);
        }

        /**
         * @brief print the student's information
         * 
         */
        void printStudent(){
            std::cout << *this << std::endl;
        }

        /**
         * @brief overloaded operator== to compare two students, return true if id's are equal
         * 
         * @param t student to compare to
         * @return bool 
         */
        bool operator== (const Student& t) const{
            return this->id == t.id;
        }

        /**
         * @brief overloaded operator!= to compare two students, return true if id's are not equal
         * 
         * @param t student to compare to
         * @return bool 
         */
        bool operator!= (const Student& t) const{
            return this->id != t.id;
        }

        /**
         * @brief overloaded operator<< to print the student's information
         * 
         * @param out  output stream
         * @param p  student object
         * @return std::ostream& 
         */
        friend std::ostream& operator<<(std::ostream &out, const Student& p){
            out << "Student ID: " << p.id << std::endl;
            out << "Student Name: " << p.name << std::endl;
            out << "Student Email: " << p.email << std::endl;
            out << "Student Presentation Grade: " << p.getPresentationGrade() << std::endl;
            out << "Student Essay Grade: " << p.getEssayGrade() << std::endl;
            out << "Student Term Project Grade: " << p.getTermProjectGrade() << std::endl;
            return out;
        }

        /**
         * @brief overloaded operator>> to read the student's information
         * 
         * @param in  input stream
         * @param p  student object
         * @return std::istream& 
         */
        friend std::istream& operator>>(std::istream &in, Student& p){
            std::cout << "Enter Student ID: ";
            in >> p.id;
            std::cout << "Enter Student Name: ";
            in >> p.name;
            std::cout << "Enter Student Email: ";
            in >> p.email;
            std::cout << "Enter Student Presentation Grade: ";
            in >> p.presentationGrade;
            std::cout << "Enter Student Essay Grade: ";
            in >> p.essayGrade;
            std::cout << "Enter Student Term Project Grade: ";
            in >> p.termProjectGrade;
            return in;
        }

        /**
         * @brief Overloaded assignment operator
         * 
         * @param s 
         * @return Student& 
         */
        Student& operator=(const Student &s){
            this->setId(s.id);
            this->setName(s.name);
            this->setEmail(s.email);
            this->setPresentationGrade(s.presentationGrade);
            this->setEssayGrade(s.essayGrade);
            this->setTermProjectGrade(s.termProjectGrade);

            return *this;
        }
};

unsigned int Student::studentCount = 0;

#endif

#ifndef CLASSROLL_HPP
#define CLASSROLL_HPP

#include <unordered_map>
#include <fstream>
#include <map>

namespace Comparator{
    /**
     * @brief used to compare students by id for sorting
     * 
     * @param s1
     * @param s2
     */
    bool compareId(Student* s1, Student* s2){
        return s1->getId() < s2->getId();
    }

    /**
     * @brief used to compare students by name for sorting
     * 
     * @param s1
     * @param s2
     */
    bool compareName(Student* s1, Student* s2){
        return s1->getName() < s2->getName();
    }

    /**
     * @brief used to compare students by email for sorting
     * 
     * @param s1
     * @param s2
     */
    bool compareEmail(Student* s1, Student* s2){
        return s1->getEmail() < s2->getEmail();
    }

    /**
     * @brief used to compare students by presentation grade for sorting
     * 
     * @param s1
     * @param s2
     */
    bool comparePresentationGrade(Student* s1, Student* s2){
        return s1->getPresentationGrade() < s2->getPresentationGrade();
    }

    /**
     * @brief used to compare students by essay grade for sorting
     * 
     * @param s1
     * @param s2
     */
    bool compareEssayGrade(Student* s1, Student* s2){
        return s1->getEssayGrade() < s2->getEssayGrade();
    }

    /**
     * @brief used to compare students by term project grade for sorting
     * 
     * @param s1
     * @param s2
     */
    bool compareTermProjectGrade(Student* s1, Student* s2){
        return s1->getTermProjectGrade() < s2->getTermProjectGrade();
    }
}

class ClassRoll{
    private:
        std::unordered_map<std::string, Student*> classRoll;
        /**
         * @brief Checks if a student exists in the class roll
         * 
         * @param id
         */
        void isValidStudent(std::string id){
            if(this->classRoll.find(id) == this->classRoll.end()){
                throw StudentExceptions::InvalidStudentException("Student with id " + id + " does not exist within System");
            }
        }

    public:

        /**
         * @brief Construct a new Class Roll:: Class Roll object
         * 
         */
        ClassRoll(){
            this->classRoll = std::unordered_map<std::string, Student*>();
        }

        /**
         * @brief Destroy the Class Roll:: Class Roll object
         * 
         */
        ~ClassRoll(){
            for(auto it = this->classRoll.begin(); it != this->classRoll.end(); it++){
                delete it->second;
            }
        }

        /**
         * @brief Adds a student to the class roll using a pointer to a student object, 
         *        checks if the student already exists
         * 
         * @param s 
         */
        void addStudent(Student* s){
            if(this->classRoll.find(s->getId()) != this->classRoll.end()){
                throw StudentExceptions::InvalidStudentException("Student with id " + s->getId() + " already exists within System");
            }
            this->classRoll[s->getId()] = s;
        }

        /**
         * @brief Adds a student to the class roll using a pointer to a student object, 
         *        checks if the student already exists
         * 
         * @param id 
         * @param name 
         * @param email 
         * @param presentationGrade 
         * @param essayGrade 
         * @param termProjectGrade 
         */
        void addStudent(std::string id, std::string name, std::string email = "", int presentationGrade = Grading::MAX_GRADE, int essayGrade = Grading::MAX_GRADE, int termProjectGrade = Grading::MAX_GRADE){
            if(this->classRoll.find(id) != this->classRoll.end()){
                throw StudentExceptions::InvalidStudentException("Student with id " + id + " already exists within System");
            }
            if(email == ""){
                email = Validation::generateEmail(Student::getStudentCount());
            }
            this->classRoll[id] = new Student(id, name, email, presentationGrade, essayGrade, termProjectGrade);
        }

        /**
         * @brief Removes a student from the class roll using the student's id
         *        checks if the student exists
         * 
         * @param id 
         */
        void removeStudent(std::string id){
            isValidStudent(id);
            Student* toDel = this->classRoll[id];
            this->classRoll.erase(id);
            delete toDel;
        }

        /**
         * @brief Prints a student's information using the student's id
         *        checks if the student exists
         * 
         * @param id 
         */
        void printStudent(std::string id){
            isValidStudent(id);
            std::cout << *this->classRoll[id] << std::endl;
        }

        /**
         * @brief Prints all students in the class roll
         * 
         */
        void printAllStudents(){
            for(auto it = this->classRoll.begin(); it != this->classRoll.end(); it++){
                std::cout << *(it->second) << std::endl;
            }
        }

        /**
         * @brief Prints all students in the class roll sorted by a given sort type
         * 
         * @param sortType 
         */
        void printAllStudents(std::string sortType){
            std::vector<Student*> students = this->getAllStudents();

            if(sortType == "name"){
                std::sort(students.begin(), students.end(), Comparator::compareName);
            }
            else if(sortType == "id"){
                std::sort(students.begin(), students.end(), Comparator::compareId);
            }
            else if(sortType == "email"){
                std::sort(students.begin(), students.end(), Comparator::compareEmail);
            }
            else if(sortType == "presentationGrade"){
                std::sort(students.begin(), students.end(), Comparator::comparePresentationGrade);
            }
            else if(sortType == "essayGrade"){
                std::sort(students.begin(), students.end(), Comparator::compareEssayGrade);
            }
            else if(sortType == "termProjectGrade"){
                std::sort(students.begin(), students.end(), Comparator::compareTermProjectGrade);
            }

            for(auto it = students.begin(); it != students.end(); it++){
                std::cout << **it << std::endl;
            }
        }

        /**
         * @brief Gets a student from the class roll using the student's id
         *        checks if the student exists
         * 
         * @param id 
         * @return Student* 
         */
        void updateStudent(std::string id, std::string name, std::string email = "", int presentationGrade = -1, int essayGrade = -1, int termProjectGrade = -1){
            isValidStudent(id);
            
            this->classRoll[id]->setName(name);
            if(email != ""){
                this->classRoll[id]->setEmail(email);
            }
            if(presentationGrade != -1){
                this->classRoll[id]->setPresentationGrade(presentationGrade);
            }
            if(essayGrade != -1){
                this->classRoll[id]->setEssayGrade(essayGrade);
            }
            if(termProjectGrade != -1){
                this->classRoll[id]->setTermProjectGrade(termProjectGrade);
            }
            
        }

        /**
         * @brief Gets a student from the class roll using the student's id
         *        checks if the student exists
         * 
         * @param id 
         * @return Student* 
         */
        void updateStudent(std::string id, Student* s){
            if(id != s->getId()){
                throw StudentExceptions::InvalidStudentException("Student id's do not match. Id given: " + id + ". Id in student object: " + s->getId());
            }
            removeStudent(id);
            addStudent(s);
        }

        /**
         * @brief Gets a student from the class roll using the student's id
         *        checks if the student exists
         * 
         * @param id 
         * @return Student* 
         */
        unsigned int getStudentCount(){
            return this->classRoll.size();
        }

        /**
         * @brief Gets a student from the class roll using the student's id
         *        checks if the student exists
         * 
         * @param id 
         * @return Student* 
         */
        Student* getStudent(std::string id){
            isValidStudent(id);
            return this->classRoll[id];
        }

        /**
         * @brief Gets all students in the class roll as a vector
         * 
         * @return std::vector<Student*> 
         */
        std::vector<Student*> getAllStudents(){
            std::vector<Student*> students;
            for(auto it = this->classRoll.begin(); it != this->classRoll.end(); it++){
                students.push_back(it->second);
            }
            return students;
        }

        /**
         * @brief implements the << operator for the ClassRoll class, printing all students in the class roll
         * 
         * @param out
         * @param p
         */
        friend std::ostream& operator<<(std::ostream &out, const ClassRoll& p){
            for(auto it = p.classRoll.begin(); it != p.classRoll.end(); it++){
                out << *(it->second) << std::endl;
            }
            return out;
        }

        /**
         * @brief implements the >> operator for the ClassRoll class, adding a student to the class roll
         * 
         * @param in
         * @param p
         */
        friend std::istream& operator>>(std::istream &in, ClassRoll& p){
            std::string id;
            std::string name;
            std::string email;
            int presentationGrade;
            int essayGrade;
            int termProjectGrade;
            
            in >> name;
            in >> id;
            in >> email;
            in >> presentationGrade;
            in >> essayGrade;
            in >> termProjectGrade;

            p.addStudent(new Student(name, id, email, presentationGrade, essayGrade, termProjectGrade));

            return in;
        }

        /**
         * @brief Saves the class roll to a file
         * 
         * @param filename 
         */
        void saveToFile(std::string fileName){
            std::ofstream file;
            file.open(fileName);
            file << *this;
            file.close();
        }

};

#endif



struct UID{
    std::string operator()(int index){
        std::string temp =  std::to_string(index);
        return std::string(9 - temp.length(), '0') + temp;
    }
};

int randomGrade(){ 
    return rand() % 5;
}

void testValidationFunctions(){ //Testing functions within Validation NameSpace
    auto name = [](std::string name){ //Lambda function to test Validation::isValidName
        std::cout << "Name: " << name << " is " << (Validation::isValidName(name) ? "" : "NOT")<< " valid" << std::endl;
    };
    auto id = [](std::string id){ //Lambda function to test Validation::isValidId
        std::cout << "Id: " << id << " is " << (Validation::isValidId(id) ? "" : "NOT")<< " valid" << std::endl;
    };
    auto email = [](std::string email){ //Lambda function to test Validation::isValidEmail
        std::cout << "Email: " << email << " is " << (Validation::isValidEmail(email) ? "" : "NOT")<< " valid" << std::endl;
    };
    auto grade = [](unsigned int grade){ //Lambda function to test Validation::isValidGrade
        std::cout << "Grade: " << grade << " is " << (Validation::isValidGrade(grade) ? "" : "NOT")<< " valid" << std::endl;
    };

    name("John Doe");
    name("John");
    name("John Smith");
    name("John Smith Jr.");
    name("John Smith Jr");
    name("John Smith Jr. III");
    name("John Smith Jr III");
    name("John Smith Jr. III IV");
    name("John Smith Jr III IV");
    name("John Smith Sr. III IV V");
    name("John Smith Sr III IV V");

    id("123456789");
    id("U123456789");
    id("U12345678");
    id("U1234567890");

    email("studenta@usf.edu");
    email("studenta@hotmail.com");
    email("studenta@gmail.com");

    grade(0);
    grade(1);
    grade(2);
    grade(3);
    grade(4);
    grade(5);
    

}

void testAllFunctions(ClassRoll* cr){ //Testing all the functions of ClassRoll and Student
    struct UID f = UID();

    for(int i = 0; i < 5; i++){
        // auto name  = "Student " + std::string((i / 26) + 1, i % 26 + 'a');
        // auto id = Validation::ID_PREFIX + f(i);
        // auto email = "student" + std::to_string(i) + "@example.com";

        std::string name = Validation::generateName(i);
        std::string id = Validation::generateId(i);
        std::string email = Validation::generateEmail(i);
        Grading::Grade presentationGrade = randomGrade();
        Grading::Grade essayGrade = randomGrade();
        Grading::Grade termProjectGrade = randomGrade();
        cr->addStudent(new Student(name, id, email, presentationGrade, essayGrade, termProjectGrade));
    }
    cr->addStudent("Student A", "U123456789");

    std::cout << "Printing all students (by id): " << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    cr->printAllStudents("id");
    std::cout << "----------------------------------------\n" << std::endl;

    std::cout << "Printing all students (by id): " << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    cr->printAllStudents("email");
    std::cout << "----------------------------------------\n" << std::endl;

    std::cout << "Printing all students (by name): " << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    cr->printAllStudents("name");
    std::cout << "----------------------------------------\n" << std::endl;

    std::cout << "Printing all students (by presentation grade): " << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    cr->printAllStudents("presentationGrade");
    std::cout << "----------------------------------------\n" << std::endl;

    std::cout << "Printing all students (by essay grade): " << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    cr->printAllStudents("essayGrade");
    std::cout << "----------------------------------------\n" << std::endl;

    std::cout << "Printing all students (by term project grade): " << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    cr->printAllStudents("termProjectGrade");
    std::cout << "----------------------------------------\n" << std::endl;

    //remove a student
    std::cout << "Removing student with id: U000000001" << std::endl;
    cr->removeStudent("U000000001");
    try{
        cr->getStudent("U000000001");
    }
    catch(StudentExceptions::InvalidStudentException& e){
        std::cout << e.what() << std::endl;
    }

    //get a student
    std::cout << "Getting student with id: U000000002" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    cr->getStudent("U000000002")->printStudent();
    std::cout << "----------------------------------------\n" << std::endl;

    //update a student incorrectly
    std::cout << "Updating student with id (incorrectly): U000000003" << std::endl;
    try{
        cr->updateStudent("U000000003", new Student("Student a", "U000000030", "student3t@usf.edu", 1, 2, 3));
    }
    catch(StudentExceptions::InvalidStudentException& e){
        std::cout << e.what() << std::endl;
    }

    std::cout << "Printing all students (by id): \n" << *cr << std::endl;
    //get a student
    std::cout << "Getting student with id: U000000003" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    cr->getStudent("U000000003")->printStudent();
    std::cout << "----------------------------------------\n" << std::endl;

    //update a student incorrectly
    std::cout << "Updating student with id (correctly): U000000003" << std::endl;
    cr->updateStudent("U000000003", new Student("Student a", "U000000003", "student3t@usf.edu", 1, 2, 3));

    std::cout << "Printing all students (by id): \n" << *cr << std::endl;
    //get a student
    std::cout << "Getting student with id: U000000003" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    cr->getStudent("U000000003")->printStudent();
    std::cout << "----------------------------------------\n" << std::endl;

    //save to file and load from file
    std::cout << "Saving to file: test.txt" << std::endl;
    cr->saveToFile("test.txt");


    testValidationFunctions();
    
}
int main(){
    
    ClassRoll* cr = new ClassRoll();
    testAllFunctions(cr);
    delete cr;

    std::cout << "Done, no errors" << std::endl;
    std::vector<Student*> students;
    int i;
    try{
        for(i = 0; i < 100; i++){
            // std::cout << "Starting Interation: "<< i << std::endl;
            std::string name = Validation::generateName(i);
            std::string id = Validation::generateId(i);
            std::string email = Validation::generateEmail(i);
            Grading::Grade presentationGrade = randomGrade();
            Grading::Grade essayGrade = randomGrade();
            Grading::Grade termProjectGrade = randomGrade();
            
            students.push_back(new Student(name, id, email, presentationGrade, essayGrade, termProjectGrade));
            // std::cout << "Finishing Interation: " << i << std::endl;
        }
    }
    catch(StudentExceptions::InvalidStudentException& e){
        std::cout << e.what() << "\n\n\nHappened at index: " << i << std::endl;
    }
    for(auto s : students){
        std::cout << *s << std::endl;
    }
    return 0;
}
