#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STUD_FILE "students.txt"
#define CRE_FILE  "credentials.txt"

char currentUser[50];
char currentRole[20];

void trimNewline(char *str) {
    str[strcspn(str, "\n")] = '\0';
}

int login() {
    char u[50], p[50], r[20];
    char inUser[50], inPass[50];

    printf("USERNAME: ");
    fgets(inUser, sizeof(inUser), stdin);
    trimNewline(inUser);

    printf("PASSWORD: ");
    fgets(inPass, sizeof(inPass), stdin);
    trimNewline(inPass);

    FILE *fp = fopen(CRE_FILE, "r");
    if (!fp) {
        printf("Credential file missing!\n");
        return 0;
    }

    while (fscanf(fp, "%s %s %s", u, p, r) == 3) {
        if (strcmp(inUser, u) == 0 && strcmp(inPass, p) == 0) {
            strcpy(currentUser, u);
            strcpy(currentRole, r);
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

void addStudent() {
    int roll;
    char name[50];
    float mark;

    printf("Roll: ");
    scanf("%d", &roll);
    getchar(); // consume newline

    printf("Name: ");
    fgets(name, sizeof(name), stdin);
    trimNewline(name);

    printf("Mark: ");
    scanf("%f", &mark);

    FILE *fp = fopen(STUD_FILE, "a");
    if (!fp) {
        printf("Error opening student file!\n");
        return;
    }
    fprintf(fp, "%d %s %.2f\n", roll, name, mark);
    fclose(fp);

    printf("Student added!\n");
}

void displayStudents() {
    FILE *fp = fopen(STUD_FILE, "r");
    if (!fp) {
        printf("No student file!\n");
        return;
    }

    int roll;
    char name[50];
    float mark;

    printf("\n%-10s %-20s %-10s\n", "Roll", "Name", "Mark");
    printf("---------------------------------------------\n");
    while (fscanf(fp, "%d %s %f", &roll, name, &mark) == 3) {
        printf("%-10d %-20s %-10.2f\n", roll, name, mark);
    }

    fclose(fp);
}

void searchStudent() {
    int find, roll;
    char name[50];
    float mark;

    printf("Enter roll to search: ");
    scanf("%d", &find);

    FILE *fp = fopen(STUD_FILE, "r");
    if (!fp) {
        printf("No student file!\n");
        return;
    }

    while (fscanf(fp, "%d %s %f", &roll, name, &mark) == 3) {
        if (roll == find) {
            printf("Found: %d %s %.2f\n", roll, name, mark);
            fclose(fp);
            return;
        }
    }
    fclose(fp);
    printf("Student not found!\n");
}

void deleteStudent() {
    int delRoll;
    printf("Enter roll to delete: ");
    scanf("%d", &delRoll);

    FILE *fp = fopen(STUD_FILE, "r");
    if (!fp) {
        printf("No student file!\n");
        return;
    }
    FILE *temp = fopen("temp.txt", "w");

    int roll;
    char name[50];
    float mark;
    int found = 0;

    while (fscanf(fp, "%d %s %f", &roll, name, &mark) == 3) {
        if (roll != delRoll) {
            fprintf(temp, "%d %s %.2f\n", roll, name, mark);
        } else {
            found = 1;
        }
    }

    fclose(fp);
    fclose(temp);

    remove(STUD_FILE);
    rename("temp.txt", STUD_FILE);

    if (found) printf("Student deleted!\n");
    else printf("Roll not found!\n");
}

void updateStudent() {
    int updateRoll;
    printf("Enter roll to update: ");
    scanf("%d", &updateRoll);

    FILE *fp = fopen(STUD_FILE, "r");
    if (!fp) {
        printf("No student file!\n");
        return;
    }
    FILE *temp = fopen("temp.txt", "w");

    int roll;
    char name[50];
    float mark;
    int found = 0;

    while (fscanf(fp, "%d %s %f", &roll, name, &mark) == 3) {
        if (roll == updateRoll) {
            found = 1;
            char newName[50];
            float newMark;

            getchar(); // consume newline
            printf("New Name: ");
            fgets(newName, sizeof(newName), stdin);
            trimNewline(newName);

            printf("New Mark: ");
            scanf("%f", &newMark);

            fprintf(temp, "%d %s %.2f\n", roll, newName, newMark);
        } else {
            fprintf(temp, "%d %s %.2f\n", roll, name, mark);
        }
    }

    fclose(fp);
    fclose(temp);

    remove(STUD_FILE);
    rename("temp.txt", STUD_FILE);

    if (found) printf("Student updated!\n");
    else printf("Roll not found!\n");
}

void menu() {
    int c;
    while (1) {
        printf("\n%s MENU\n", currentRole);
        if (strcmp(currentRole, "admin") == 0) {
            printf("1.Add\n2.Display\n3.Search\n4.Update\n5.Delete\n6.Logout\n");
        } else if (strcmp(currentRole, "staff") == 0) {
            printf("1.Add\n2.Display\n3.Search\n4.Update\n5.Logout\n");
        } else {
            printf("1.Display\n2.Search\n3.Logout\n");
        }

        scanf("%d", &c);

        if (strcmp(currentRole, "admin") == 0) {
            if (c==1) addStudent();
            else if (c==2) displayStudents();
            else if (c==3) searchStudent();
            else if (c==4) updateStudent();
            else if (c==5) deleteStudent();
            else return;
        } else if (strcmp(currentRole, "staff") == 0) {
            if (c==1) addStudent();
            else if (c==2) displayStudents();
            else if (c==3) searchStudent();
            else if (c==4) updateStudent();
            else return;
        } else {
            if (c==1) displayStudents();
            else if (c==2) searchStudent();
            else return;
        }
    }
}

int main() {
    if (!login()) {
        printf("Invalid login!\n");
        return 0;
    }

    printf("Logged in as: %s\n", currentRole);
    menu();

    return 0;
}

