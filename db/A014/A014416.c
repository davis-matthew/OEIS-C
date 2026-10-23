/*
 * A014416  Inverse of 407th cyclotomic polynomial.
 * https://oeis.org/A014416
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of x^n in the formal power series expansion
 *            of 1/Φ_407(x)
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is periodic with period 407.  Computing the first 407 coefficients once
   and using a modulo operation on the index produces all terms. */

#include <stdbool.h>

static seq_t F[361];           /* coefficients of Φ_407(x), degree 360 */
static seq_t G[407];           /* first 407 coefficients of 1/Φ_407(x) */
static bool init_done = false;

/* Compute Φ_407(x) via (x^407-1)(x-1)/((x^11-1)(x^37-1)) and then the inverse series. */
static void init(void) {
    if (init_done) return;
    init_done = true;

    /* Numerator N(x) = (x^407 - 1)*(x - 1) = x^408 - x^407 - x + 1 */
    static seq_t N[409];
    static seq_t D[49];

    for (int i = 0; i < 409; ++i) N[i] = 0;
    for (int i = 0; i < 49;  ++i) D[i] = 0;

    N[408] = 1;
    N[407] = -1;
    N[1]   = -1;
    N[0]   = 1;

    /* Denominator D(x) = (x^11 - 1)*(x^37 - 1) = x^48 - x^11 - x^37 + 1 */
    D[48] = 1;
    D[37] = -1;
    D[11] = -1;
    D[0]  = 1;

    const int degN = 408;
    const int degD = 48;

    for (int shift = degN - degD; shift >= 0; --shift) {
        seq_t coef = N[shift + degD];   /* leading coefficient of current N */
        F[shift] = coef;
        if (coef != 0) {
            for (int i = 0; i <= degD; ++i) {
                N[shift + i] -= coef * D[i];
            }
        }
    }

    /* Invert series: G[0] = 1 (since F[0] = 1) */
    G[0] = 1;
    for (int n = 1; n < 407; ++n) {
        seq_t sum = 0;
        int maxk = n < 360 ? n : 360;
        for (int k = 1; k <= maxk; ++k) {
            sum += F[k] * G[n - k];
        }
        G[n] = -sum;
    }
}

seq_t a(idx_t n) {
    init();
    idx_t pos = n % 407;
    return G[(int)pos];
}
