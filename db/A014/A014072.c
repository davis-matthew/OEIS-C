/*
 * A014072  Inverse of 63rd cyclotomic polynomial.
 * https://oeis.org/A014072
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of x^n in the power series expansion of
 *            1/Φ_63(x)
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    /* static data */
    static bool coeff_done = false, cache_done = false;
    static seq_t coeff[37];        /* coefficients of Φ_63(x), c[0] = 1 */
    static seq_t cache[63];        /* first 63 terms of the sequence */

    /* ------------------------------------------------------------------ */
    /* Helper to compute Φ_63(x) coefficients by exact polynomial division */
    /* ------------------------------------------------------------------ */
    if (!coeff_done) {
        /* Build numerator N(x) = (x^63-1)(x^3-1) = x^66 - x^63 - x^3 + 1 */
        const int degN = 66;
        seq_t N[67] = {0};
        N[degN]   = 1;   /* x^66 */
        N[63]     = -1;  /* -x^63 */
        N[3]      = -1;  /* -x^3 */
        N[0]      = 1;   /* +1 */

        /* Build denominator D(x) = (x^21-1)(x^9-1) = x^30 - x^21 - x^9 + 1 */
        const int degD = 30;
        seq_t D[31] = {0};
        D[degD]   = 1;   /* x^30 */
        D[21]     = -1;  /* -x^21 */
        D[9]      = -1;  /* -x^9 */
        D[0]      = 1;   /* +1 */

        /* Long division N / D  (D is monic) to obtain Φ_63(x) */
        const int degQ = degN - degD;      /* 36 */
        seq_t Q[37] = {0};
        seq_t R[67];
        for (int i = 0; i <= degN; ++i) R[i] = N[i];

        for (int i = degN; i >= degD; --i) {
            seq_t c = R[i];
            Q[i - degD] = c;           /* coefficient of x^(i-degD) */
            if (c != 0) {
                for (int j = 0; j <= degD; ++j) {
                    R[(i - degD) + j] -= c * D[j];
                }
            }
        }
        /* Store coefficients c[0..36] */
        for (int i = 0; i <= degQ; ++i) coeff[i] = Q[i];
        coeff_done = true;
    }

    /* ------------------------------------------------------------------ */
    /* Compute first 63 terms using the recurrence a_n = - Σ_{k=1}^{36} c_k * a_{n-k} */
    /* ------------------------------------------------------------------ */
    if (!cache_done) {
        cache[0] = 1;  /* a(0) = 1 */
        for (int i = 1; i < 63; ++i) {
            seq_t sum = 0;
            int kmax = i < 36 ? i : 36;
            for (int k = 1; k <= kmax; ++k) {
                sum += coeff[k] * cache[i - k];
            }
            cache[i] = -sum;  /* a_n = -Σ coeff_k * a_{n-k} */
        }
        cache_done = true;
    }

    /* Periodic with period 63: return cached value */
    idx_t r = n % 63;
    return cache[(int)r];
}
