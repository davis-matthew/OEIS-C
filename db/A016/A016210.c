/*
 * A016210  Inverse of 2201st cyclotomic polynomial.
 * https://oeis.org/A016210
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of x^n in the power series expansion of 1 /
 *            Φ₍₂₂₀₁₎(x). The sequence is periodic with period 2201
 * Range:     i8 n <= 3000, i16 n <= 3000, i32 n <= 3000, i64 n <= 3000,
 *            i128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    /* static initialization flag */
    static bool initialized = false;
    /* period of the sequence */
    const idx_t period = 2201;
    /* Coefficients of Φ₍₂₂₀₁₎(x), degree 2100 */
    static seq_t coeff[2101];
    /* Precomputed sequence values for one period */
    static seq_t seq[2201];

    if (!initialized) {
        /* --------- Compute Φ₍₂₂₀₁₎(x) by rational function division -------- */
        const int degQ   = 102;   /* degree of denominator (x^71-1)(x^31-1) */
        const int totalDeg = 2202; /* degree of numerator (x^2201-1)(x-1) */

        /* Numerator coefficients: x^2202 - x^2201 - x + 1 */
        seq_t P[2203] = {0};
        P[0] = 1;
        P[1] = -1;
        P[2201] = -1;
        P[2202] = 1;

        /* Denominator coefficients: x^102 - x^71 - x^31 + 1 */
        seq_t Q[103] = {0};
        Q[0] = 1;
        Q[31] = -1;
        Q[71] = -1;
        Q[102] = 1;

        /* Long division: P = Q * R, find R (coeff array) */
        for (int i = totalDeg; i >= degQ; --i) {
            seq_t c = P[i];
            if (c != 0) {
                int rindex = i - degQ;          /* coefficient index in R */
                coeff[rindex] = c;
                /* subtract c * Q from the appropriate part of P */
                for (int j = 0; j <= degQ; ++j) {
                    P[rindex + j] -= Q[j] * c;
                }
            }
        }

        /* --------- Compute the inverse series a[n] up to one period -------- */
        seq[0] = 1;                            /* a[0] = 1 */
        for (int n = 1; n <= 2200; ++n) {
            seq_t sum = 0;
            int maxk = (n <= 2100) ? n : 2100;
            for (int k = 1; k <= maxk; ++k) {
                sum += coeff[k] * seq[n - k];
            }
            seq[n] = -sum;                     /* negative because Φ[0] == 1 */
        }
        initialized = true;
    }

    /* Return the value according to the period */
    idx_t idx = n % period;
    return seq[idx];
}
