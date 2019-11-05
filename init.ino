#include <Time.h>
#include <TimeLib.h>
#include <math.h>

const int control = 9, soundpin = A0;
boolean motor = false, sensorSonido = false;
char instruccion[1024];
int i = 0, a = 0, adc_num = 0;

time_t fecha;

void setup()
{
  Serial.begin(9600);
  pinMode(soundpin, INPUT);
  pinMode(control, OUTPUT) ;
}

void loop() {

  if (Serial.available() > 0) {
    char dato = Serial.read();
    instruccion[i++] = dato;

    if (strstr(instruccion, "1") && sensorSonido == false) {
      Serial.write("1");
      analogWrite (control, 255) ;
      clean();
    }

    if (strstr(instruccion, "0") && sensorSonido == false) {
      Serial.write("0");
      analogWrite (control, 0) ;
      clean();
    }

  }

  while (Serial.available() <= 0) {

    int adc = analogRead(soundpin);  // entrada de sonido

    if (adc > 700) {
      if (adc_num == 0) {
        fecha = now();
      }
      Serial.print("adc_num: ");
      Serial.print(adc_num);
      Serial.print("\n");
      adc_num = adc_num + 1;
      if (now() - fecha > 10) {
        adc_num = 0;
      }
      if (adc_num > 500 && ((now() - fecha) < 10)) {
        break;
      }
    }
  }

  if (adc_num > 500 && ((now() - fecha) < 10)) {

    sensorSonido == true;
    Serial.print("NUMERO DE ADCS:");
    Serial.print(adc_num);
    Serial.print("\n");
    adc_num = 0;

    for (int i = 0; i < 2; i++) { //numero de vueltas
      motor = true;
      Serial.print("motor encendido \n");
      for (int n = 0; n < 255; n++) {
        analogWrite (control, n) ; //velocidad regulada
        delay(30); //tiempo de la pausa
      }
      delay(50); //retardo entre vueltas
    }
    motor = false; //APAGAMOS EL MOTOR
  }
  else {
    adc_num = 0;
  }

  if ((motor == false) && (sensorSonido = true)) {
    analogWrite (control, LOW);
    sensorSonido = false;
    adc_num = 0;
  }

  //  if (motorApagado == true) {
  //    analogWrite (control, LOW);
  //    do {
  //
  //    } while (analogRead(soundpin) > 700);
  //    motorApagado = false; //el motor deja de estar apagado
  //  }
  //}
  //}
}

void clean()
{
  for (int cl = 0; cl < i; cl++) {
    instruccion[cl] = 0;
  }

  i = 0;

}
