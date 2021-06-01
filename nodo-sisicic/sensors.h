/**
    Header que contiene funcionalidades referidas a los sensores conectados.
    @file sensors.h
    @author Franco Abosso
    @author Julio Donadello
    @version 1.0 29/03/2021
*/

/**
    refreshAllSensors() se encarga de pedir el refresco de todos los sensores,
    poniendo cada uno de los flags del vector refreshRequested en true.
*/
void refreshAllSensors() {
    for (int i = 0; i < SENSORS_QTY; i++) {
        refreshRequested[i] = true;
    }
    #if DEBUG_LEVEL >= 2
        Serial.println("Refrescando sensores!");
    #endif
}

/**
    stopRefreshingAllSensors() se encarga de parar el refresco de todos los sensores,
    poniendo cada uno de los flags del vector refreshRequested en false.
*/
void stopRefreshingAllSensors() {
    for (int i = 0; i < SENSORS_QTY; i++) {
        refreshRequested[i] = false;
    }
    #if DEBUG_LEVEL >= 2
        Serial.println("Abandonando refrescos!");
    #endif
}

/**
    getNewVoltage() se encarga de agregar un nuevo valor en el array de medición de tensión.
    Luego de hacerlo, baja el flag correspondiente en refreshRequested.
*/

void getNewCurrent() {
    float newCurrent = 0.0;
    if (index < ARRAY_SIZE) {
        #ifndef CORRIENTE_MOCK
            newCurrent = eMon.Irms;
            currents[index] = newCurrent;
        #else
            newCurrent = CORRIENTE_MOCK + random(30) / 100.0;
            currents[index] = newCurrent;
        #endif
        #if DEBUG_LEVEL >= 3
            Serial.print("Nueva corriente: ");
            Serial.println(newCurrent);
        #endif
    }
    refreshRequested[0] = false;
}

void getNewRaindrop() {
    #ifndef RAINDROP_MOCK
        if (index < ARRAY_SIZE) {
            if (analogRead(LLUVIA_PIN) >= LLUVIA_THRESHOLD_10BIT) {
                #if LLUVIA_ACTIVO == HIGH
                    raindrops[index] = true;
                #else
                    raindrops[index] = false;
                #endif
            } else {
                #if LLUVIA_ACTIVO == HIGH
                    raindrops[index] = false;
                #else
                    raindrops[index] = true;
                #endif
            }
        }
    #endif
    refreshRequested[1] = false;
}

void getNewGas() {
    float dist = 0.0;
    float height = 0.0;
    #ifndef GAS_MOCK
        dist = sonar.ping_median(PING_SAMPLES);
        dist = sonar.convert_cm(dist);
        if (dist < MIN_DISTANCE) {
            gas = float(CAPACIDAD_COMBUSTIBLE);
        } else {
            height = MAX_DISTANCE - dist;
            gas = PI_TIMES_R_SQUARED * height;
        }

        #if DEBUG_LEVEL >= 4
            Serial.println(String(dist) + " cm");
        #elif DEBUG_LEVEL >= 3
            Serial.println("Nueva nafta: " + currentStates[5]);
        #endif
    #endif
    gasRequested = false;
}

void getNewGPS() {
    #ifndef GPS_MOCK
        while (ssGPS.available() > 0) {
            GPS.encode(ssGPS.read());
        }
        GPSRequested = false;
    #endif
}