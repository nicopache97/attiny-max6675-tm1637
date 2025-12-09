/*
    Este programa es para leer la temperatura de un sensor MAX6675
    y mostrarla en un display 7 segmentos TM1637
    Esta probado en un ATtiny13A, pero es compatible con toda la familia ATtiny
    Uso la libreria: github.com/lpodkalicki/attiny-tm1637-library
    Creado por: github.com/nicopache97
    Rev 1.0 | 8 Dic 2025
*/

#include "tm1637.h" // modulo para el display 7 segmentos TM1637

// Pines MAX6675
#define SCKtc 3   //pin2
#define SO 4      //pin3
#define CS 2      //pin7

void setup() {
    pinMode(SCKtc, OUTPUT); // configurar pines sensor max6675
    pinMode(CS, OUTPUT);
    pinMode(SO, INPUT);
    delay(100);
    TM1637_init();
    TM1637_set_brightness(1); // ajustar brillo del display (1 a 7)
}

void loop()
{
    unsigned int temperatura, acarreo, digito;
    unsigned int readH, readL;   //variables para leer max6675
    unsigned char i, v;  // contador de bubles y buffer de bits
    // proceso de lectura del sensor max6675
    readH=0;
    readL=0;
    digitalWrite(CS, LOW);
    for(i=0;i<8;i++){
        digitalWrite(SCKtc, HIGH); delayMicroseconds(5);
        v=digitalRead(SO);
        readH += v<<(7-i);
        digitalWrite(SCKtc, LOW); delayMicroseconds(5);
    }
    for(i=0;i<8;i++){
        digitalWrite(SCKtc, HIGH); delayMicroseconds(5);
        v=digitalRead(SO);
        readL += v<<(7-i);
        digitalWrite(SCKtc, LOW); delayMicroseconds(5);
    }
    digitalWrite(CS, HIGH);
    temperatura=(readL >> 5) + (readH << 3);  //calculate temperature

    // proceso de conversion a digitos
    digito = temperatura % 10; // obtener la unidad
    acarreo = (temperatura / 10); // guardar el resto del numero
    if(digito!=0){
        TM1637_display_digit(TM1637_SET_ADR_03H, digito); // escribir digito en display en la ultima posicion
    }else{
        TM1637_display_segments(TM1637_SET_ADR_03H, 0); // borrar el digito
    }
    digito = acarreo % 10; // obtener la decena
    acarreo = (acarreo / 10); // guardar el resto del numero
    TM1637_display_digit(TM1637_SET_ADR_02H, digito); // escribir digito en display en la tercera posicion
    digito = acarreo % 10; // obtener la centena
    acarreo = (acarreo / 10); 
    TM1637_display_digit(TM1637_SET_ADR_01H, digito); // escribir digito en display en la segunda posicion
    TM1637_display_digit(TM1637_SET_ADR_00H, acarreo); // escribir digito en display en la primera posicion

    delay(500);
}
