/*
 * Created by: Emerson Medeiros Noberto
 * 18/10/23
 * 
 * 
 * 
 */

#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include "RTClib.h"

RTC_DS1307 rtc;
File myFile;
const int AOUTpin = 0;
int limit;
int value;
int ledvermelho = 2;
int ledverde = 3;
int ledazul = 5;

void setup() {
  Serial.begin(9600);
  pinMode (ledvermelho, OUTPUT);
  pinMode (ledverde, OUTPUT);
  pinMode (ledazul, OUTPUT);

  Serial.print("Initializing SD card...");
  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");

  myFile = SD.open("dadosCO.txt", FILE_WRITE);

  if (myFile) {
    Serial.println("File opened successfully.");
    myFile.println("CO Values:");
  } else {
    Serial.println("Error opening file.");
  }

  if (! rtc.begin()) {
    Serial.println("Não foi possível encontrar o RTC");
    while (1);
  }

  // Ano, Mês, Dia, Horas, Minutos, Segundos
  //rtc.adjust(DateTime(2023, 10, 8, 11, 18, 0));
  Serial.println("Dia;Horas;Monóxido de carbono");
  myFile.println("Dia;Horas;Monóxido de carbono");
}


void loop() {
  DateTime now = rtc.now();
  value = analogRead(AOUTpin);
  
  // Mapeia o valor de 0 a 1023 para a faixa de 0 a 999
  int mappedValue = map(value, 0, 1023, 0, 999);

  if (value < 30) {
    digitalWrite (ledvermelho, LOW);
    digitalWrite (ledverde, HIGH);
    digitalWrite (ledazul, LOW);
  } else if (value > 30 && value < 50) {
    digitalWrite (ledvermelho, HIGH);
    digitalWrite (ledverde, HIGH);
    digitalWrite (ledazul, LOW);
  } else if (value > 50 ) {
    digitalWrite (ledvermelho, HIGH);
    digitalWrite (ledverde, LOW);
    digitalWrite (ledazul, LOW);
  }

  //=============Arquivo para Planilhas=================
  //Dia,Horas,Monóxido de carbono
  if (myFile) {
    myFile.print(now.day(), DEC);
    myFile.print("/");
    myFile.print(now.month(), DEC);
    myFile.print(";");

    myFile.print(now.hour(), DEC);
    myFile.print(":");
    myFile.print(now.minute(), DEC);
    myFile.print(";");

    myFile.println(mappedValue);
    myFile.flush(); // Certifica-se de que os dados sejam gravados no cartão SD
  } else {
    Serial.println("Error writing to file.");
  }
  //====================================================
    //=============Monitor Serial=================
    Serial.print(now.day(), DEC);
    Serial.print("/");
    Serial.print(now.month(), DEC);
    Serial.print(";");

    Serial.print(now.hour(), DEC);
    Serial.print(":");
    Serial.print(now.minute(), DEC);
    Serial.print(";");

    Serial.println(mappedValue);
  //====================================================
  delay(1000);
}
