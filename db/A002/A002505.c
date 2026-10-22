/*
 * A002505  Nearest integer to the n-th Gram point.
 * https://oeis.org/A002505
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round(g_n), where the Gram point g_n solves theta(g_n) =
 *            n*Pi, with theta(t) = (t/2)*log(t/(2*Pi)) - t/2 - Pi/8 + 1/(48t)
 *            + 7/(5760t^3) + 31/(80640t^5) + ... (Riemann-Siegel theta).
 *            Solved by Newton's method, starting from 2*Pi*exp(1 +
 *            LambertW((8n+1)/(8e)))
 * Range:     u8 n <= 109, u16 n <= 3000, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   a002505_lambert_w
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>
#include "../utils/a002505_lambert_w.h"

#define A002505_PI 3.14159265358979323846
#define A002505_E  2.71828182845904523536

static double a002505_theta(double t) {
    double t2 = t * t, t3 = t2 * t, t5 = t3 * t2;
    return 0.5 * t * log(t / (2.0 * A002505_PI)) - 0.5 * t - A002505_PI / 8.0
         + 1.0 / (48.0 * t) + 7.0 / (5760.0 * t3) + 31.0 / (80640.0 * t5);
}

static double a002505_dtheta(double t) {
    double t2 = t * t, t4 = t2 * t2, t6 = t4 * t2;
    return 0.5 * log(t / (2.0 * A002505_PI))
         - 1.0 / (48.0 * t2) - 21.0 / (5760.0 * t4) - 155.0 / (80640.0 * t6);
}

seq_t a(idx_t n) {
    double nd = (double)n;
    double target = nd * A002505_PI;
    double t = 2.0 * A002505_PI *
               exp(1.0 + a002505_lambert_w((8.0 * nd + 1.0) / (8.0 * A002505_E)));
    int i;
    if (t < 10.0) t = 10.0;
    for (i = 0; i < 100; i++) {
        double delta = (a002505_theta(t) - target) / a002505_dtheta(t);
        t -= delta;
        if (t < 8.0) t = 8.0;
        if (fabs(delta) < 1e-12 * (1.0 + t)) break;
    }
    return (seq_t)(uint64_t)floor(t + 0.5);
}
