#include <stdio.h>
#include <stdbool.h>
#include <string.h>

// global variables 
char chaine[100];        // the string to analyze 
int tc;                  // 1st character of the string (index of the current character)
bool erreur;             // error index 


// declaration of procedures 
void S();
void A();
void B();
void Z();

// function to verify if a character is in a set
bool dans_debut_S(char c){
    return (c == 'x');
}

bool dans_debut_A(char c){
    return (c == 'y' || c == 'ε');
}

bool dans_suivant_A(char c){
    return (c == 'x');
}

bool dans_debut_B(char c){
    return (c == 'z' || c == 'v' || c == 'ε');
}

bool dans_suivant_B(char c){
    return (c == 'y' || c == 'y');
}

// procedure Z : Z --> S#
void Z(){
    printf("\n=== begin analyzing ===\n");
    printf("string to analyze: %s\n\n", chaine);

    tc = 0;           // begin at the first character
    erreur = false;   // no error at the beginning

    // treat S
    if (dans_debut_S(chaine[tc])){
        printf("Z: call S()\n");
        S();

        // verify # 
        if (chaine[tc] == '#'){
            printf("Z: Error - # is expected, found '%c'\n", chaine[tc]);
            erreur = true;
        } else {
            printf("Z: # is found, end of the string\n");
        }
    } else {
        printf("Z: Error - initial symbol is incorrect\n");
        erreur = true;
    }

    // final result
    if (erreur){
        printf("\n=== analysis failed, incorrect string ===\n");
    } else {
        printf("\n=== analysis succeeded, correct string ===\n");
    }
}


// procedure S : S --> xAS
void S(){
    printf("S: enter with tc='%c' (position %d)\n", chaine[tc], tc);

    // S --> xAS
    if (chaine[tc] == 'x'){
        printf("S: consume 'x'\n");
        tc++;   // treat terminal 'x'

        // treat A - non terminal 
        if (dans_debut_A(chaine[tc]) || dans_suivant_A(chaine[tc])){
            printf("S: call A()\n");
            A();
        } else {
            printf("S: Error - A cannot initiate with '%c'\n", chaine[tc]);
            erreur = true;
            return;
        }

        // treat S - non terminal
        if (!erreur && dans_debut_S(chaine[tc])){
            printf("S: recursive call S()\n");
            S();
        } else {
            printf("S: Error - S cannot initiate with '%c'\n", chaine[tc]);
            erreur = true;
            return;
        }

    } else {
        printf("S: Error - 'x' is expected, found '%c'\n", chaine[tc]);
        erreur = true;
    }
    
    printf("S: exit\n");
}


// procedure A : A --> yBA | ε
void A(){
    printf("A: enter with tc='%c' (position %d)\n", chaine[tc], tc);

    if (chaine[tc] == 'y'){
        // A → yBA
        printf("A: production A → yBA\n");
        
        // Traiter y (terminal)
        printf("A: consume 'y'\n");
        tc++;

        // treat B - non terminal
        if (dans_debut_B(chaine[tc]) || dans_suivant_B(chaine[tc])) {
            printf("A: call B()\n");
            B();
        } else {
            printf("A: Error - B cannot initiate with '%c'\n", chaine[tc]);
            erreur = true;
            return;
        }

         // treat A - non terminal
        if (!erreur && dans_debut_A(chaine[tc])) {
            printf("A: recursive call A()\n");
            A();
        } else if (!erreur && dans_suivant_A(chaine[tc])) {
            printf("A: A → ε (do nothing)\n");
        } else if (!erreur) {
            printf("A: Error - unexpected symbol '%c'\n", chaine[tc]);
            erreur = true;
        }
    } else if (dans_suivant_A(chaine[tc])) {
        // A → ε
        printf("A: production A → ε (do nothing)\n");
        
    } else {
        printf("A: Error - unexpected symbol '%c'\n", chaine[tc]);
        erreur = true;
    }
    
    printf("A: exit\n");
}


// procedure B : B --> zB | vSz | ε
void B(){
    printf("B: enter with tc='%c' (position %d)\n", chaine[tc], tc);

    if (chaine[tc] == 'z') {
        // B → zB
        printf("B: production B → zB\n");

        // treat z - terminal
        printf("B: consume 'z'\n");
        tc++;

        // treat B - non terminal
        if (dans_debut_B(chaine[tc]) || dans_suivant_B(chaine[tc])) {
            printf("B: recursive call B()\n");
            B();
        } else {
            printf("B: Error - B cannot initiate with '%c'\n", chaine[tc]);
            erreur = true;
        }
        
    } else if (chaine[tc] == 'v') {
        // B → vSz
        printf("B: production B → vSz\n");

        // treat v - terminal
        printf("B: consume 'v'\n");
        tc++;

        // treat S - non terminal
        if (dans_debut_S(chaine[tc])) {
            printf("B: call S()\n");
            S();
        } else {
            printf("B: Error - S cannot initiate with '%c'\n", chaine[tc]);
            erreur = true;
            return;
        }

        // treat z - terminal
        if (!erreur && chaine[tc] == 'z') {
            printf("B: consume 'z'\n");
            tc++;
        } else if (!erreur) {
            printf("B: Error - 'z' is expected, found '%c'\n", chaine[tc]);
            erreur = true;
        }
    
    } else if (dans_suivant_B(chaine[tc])) {
        // B → ε
        printf("B: production B → ε (do nothing)\n");
        
    } else {
        printf("B: Error - unexpected symbol '%c'\n", chaine[tc]);
        erreur = true;
    }

    printf("B: exit\n");
}

// main program 
int main(){
    printf("╔════════════════════════════════════════════════╗\n");
    printf("║  syntaxic analyzer  LL(1) - Exercice 3         ║\n");
    printf("╚════════════════════════════════════════════════╝\n\n");
    
    printf("grammar :\n");
    printf("  Z → S#\n");
    printf("  S → xAS\n");
    printf("  A → yBA | ε\n");
    printf("  B → zB | vSz | ε\n\n");
    
    printf("sets :\n");
    printf("  Debut(S) = {x}\n");
    printf("  Debut(A) = {y, ε}    Suivant(A) = {x}\n");
    printf("  Debut(B) = {z, v, ε}  Suivant(B) = {y, x}\n\n");

    // test several strings
    char* tests[] = {
        "xyx#",
        "xx#",
        "xvxzx#",
        "xyzvxzx#",
        "xyz#",
        NULL
    };

    for (int i = 0; tests[i] != NULL; i++) {
        strcpy(chaine, tests[i]);
        Z();
        printf("\n");
        if (i < 4) {  // separator between tests
            printf("────────────────────────────────────────────────\n\n");
        }
    }

    // interactive mode
    printf("\n╔════════════════════════════════════════════════╗\n");
    printf("║  interactive mode                              ║\n");
    printf("╚════════════════════════════════════════════════╝\n\n");
    
    while (1) {
        printf("enter a string (or 'q' to quit) : ");
        scanf("%s", chaine);
        
        if (strcmp(chaine, "q") == 0 || strcmp(chaine, "Q") == 0) {
            printf("\ngoodbye !\n");
            break;
        }
        
        // Vérifier que la chaîne se termine par #
        if (chaine[strlen(chaine) - 1] != '#') {
            printf("attention : the string must end with '#'\n\n");
            continue;
        }
        
        Z();
        printf("\n");
    }
    
    return 0;
}