#include <iostream>
#include <string>
#include <windows.h>
using namespace std;

//CIFRADO CESAR USANDO C++ y ENSAMBLADOR 8086 PROYECTO FINAL

void EncriptadoCesar(string& texto, int Desplaza, bool desencriptar = false) {
    // Si estamos en modo descifrado, invertimos el desplazamiento
    if (desencriptar) {
        Desplaza = -Desplaza;
    }

    char Desplaza8 = static_cast<char>(Desplaza);  // Variable de 8 bits (char) para el desplazamiento
    int ciclo = 0;

    for (size_t ciclo = 0; ciclo < texto.size(); ciclo++) {
        
        char caracter = texto[ciclo];

        __asm {
            mov al, caracter //Cargar el carácter actual en AL
            mov cl, Desplaza8 //Cargar el desplazamiento en CL usando Desplaza8 (de 8 bits)

                //COMPROBAR SI ES UNA LETRA MAYÚSCULA(A - Z)
                cmp al, 'A'
                jl NoDesplazamiento //Si es menor a 'A', no se hace desplazamiento
                cmp al, 'Z'
                jg ChecarMinuscula //Si es mayor a 'Z', se comprueba si es minúscula

                add al, cl //Sumar o restar el desplazamiento para mayúsculas
                cmp al, 'A'
                jge EnRangoMayusculas  //Si está en el rango, continuar
                add al, 26 //Ajustar si se sale por debajo de 'A'
                jmp FinalDesplazamiento

                    EnRangoMayusculas :
                    cmp al, 'Z'
                    jle FinalDesplazamiento //Si está en el rango, continuar
                    sub al, 26 //Ajustar si se pasa de 'Z'
                    jmp FinalDesplazamiento

                //COMPROBAR SI ES UNA LETRA MINÚSCULA(A - Z)
                ChecarMinuscula :
                cmp al, 'a'
                jl NoDesplazamiento //Si es menor a 'a', no se hace desplazamiento
                cmp al, 'z'
                jg NoDesplazamiento //Si es mayor a 'z', no se hace desplazamiento

                add al, cl //Sumar o restar el desplazamiento para minúsculas
                cmp al, 'a'
                jge EnRangoMinusculas //Si está en el rango, continuar
                add al, 26 //Ajustar si se sale por debajo de 'a'
                jmp FinalDesplazamiento

                    EnRangoMinusculas :
                    cmp al, 'z'
                    jle FinalDesplazamiento //Si está en el rango, continuar
                    sub al, 26 //Ajustar si se pasa de 'z'
                    jmp FinalDesplazamiento

                NoDesplazamiento :
                mov al, caracter //No modificar caracteres fuera de letras

                FinalDesplazamiento :
                mov caracter, al //Guardar el carácter cifrado / descifrado
        }

        texto[ciclo] = caracter;  // Asignar el carácter procesado a la posición actual del ciclo
    }
}

//-----------------------------CODIGO CLIENTE-------------------------------------------------//
int main() {
    string texto;
    int Desplaza;
    char opcion;
    cout<<"-----------------[ CIFRADO CESAR ]-------------------\n\n";
    cout << "Ingrese el texto: ";
    getline(cin, texto);

    while (true) {
        cout << "Ingrese el desplazamiento (por ejemplo, 3): ";
        //VERIFICACION DE VALOR NUMERICO
        if (!(cin >> Desplaza)) {
            cout << "\nEntrada invalida. Por favor ingrese un numero.\n";
            cin.clear();                //Limpieza de Cin
            cin.ignore(10000, '\n');    //Limpieza de Cin
        }
        //VERIFICACION DE QUE ESTE DENTRO DE LA EXTENCION DEL ABECEDARIO INGLES
        else if (Desplaza < 1 || Desplaza > 26) {
            cout << "\nPor favor, ingrese un numero entre 1 y 26.\n";
        }
        else {
            break;
        }
    }

    while (true) {
        cout << "Desea cifrar o descifrar? (c/d): ";
        cin >> opcion;

        // VERIFICACION DE SI LA OPCION INGRESADA ES VALIDA
        if (opcion == 'c' || opcion == 'C' || opcion == 'd' || opcion == 'D') {
            break;
        }
        else {
            cout << "Opción inválida. Por favor ingrese 'c' para cifrar o 'd' para descifrar.\n";
        }
    }

    //SEGUNDA VERIFICACION DE EXTENCION
    Desplaza %= 26;

    bool desencriptar = (opcion == 'd' || opcion == 'D');

    //INVOCACION DE CODIGO ENSAMBLADOR 
    EncriptadoCesar(texto, Desplaza, desencriptar);

    //ESCRIBE UNO U OTRO DEPENDIENDO DE SI SE CIFRA O DESCIFRA
    cout << (desencriptar ? "Texto descifrado: " : "Texto cifrado: ") << texto << endl;

    return 0;
}
//-----------------------------FIN CODIGO CLIENTE---------------------------------------------//

