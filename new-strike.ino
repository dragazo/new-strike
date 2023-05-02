#include <Adafruit_BNO08x.h>

#define BNO08X_CS 12
#define BNO08X_INT 39
#define BNO08X_RESET 15

Adafruit_BNO08x bno08x(BNO08X_RESET);
sh2_SensorValue_t sensorValue;

void panic(const char *msg) {
  Serial.println(msg);
  while (1) delay(10);
}

void enableReport(int kind) {
  if (!bno08x.enableReport(kind)) {
    panic("failed to enable report");
  }
}
void setReports() {
  enableReport(SH2_GAME_ROTATION_VECTOR);
}

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);

  Serial.println("connecting to BNO08x...");
  if (!bno08x.begin_SPI(BNO08X_CS, BNO08X_INT)) {
    panic("failed to connect to BNO08x");
  }
  Serial.println("connection success");
  
  setReports();
  delay(100);
}

void loop() {
  delay(10);

  if (bno08x.wasReset()) {
    Serial.println("BNO08x was reset");
    setReports();
  }

  if (!bno08x.getSensorEvent(&sensorValue)) {
    return;
  }

  switch (sensorValue.sensorId) {
    case SH2_GAME_ROTATION_VECTOR:
      Serial.print("Game Rotation Vector - r: ");
      Serial.print(sensorValue.un.gameRotationVector.real);
      Serial.print(" i: ");
      Serial.print(sensorValue.un.gameRotationVector.i);
      Serial.print(" j: ");
      Serial.print(sensorValue.un.gameRotationVector.j);
      Serial.print(" k: ");
      Serial.println(sensorValue.un.gameRotationVector.k);
      break;
  }
}
