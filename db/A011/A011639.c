/*
 * A011639  65th cyclotomic polynomial.
 * https://oeis.org/A011639
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   a(n) = coefficient of x^n in the 65th cyclotomic polynomial
 *            Φ_65(x)
 * Range:     i8 n <= 48, i16 n <= 48, i32 n <= 48, i64 n <= 48, i128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    /*  Φ_65(x) has degree φ(65)=48, hence 49 coefficients (0 … 48).  */
    static seq_t coeff[49];
    static bool initialized = false;

    if (!initialized) {
        /* 1.  Form the polynomial  x^65 – 1.  */
        /*   We do not need the full polynomial: the quotient by (x‑1)
          is simply 1 + x + x^2 + … + x^64.  */
        seq_t a_half[65];          /* degree 0 … 64 */
        for (idx_t i = 0; i < 65; ++i)
            a_half[i] = (seq_t)1;  /* coefficient of x^i is 1 */

        /* 2.  Divide by Φ_5(x) = 1 + x + x² + x³ + x⁴  (monic).  */
        const seq_t B[5] = {(seq_t)1, (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)1};
        const idx_t d5 = 4;           /* deg Φ_5 = 4 */

        seq_t temp[65];
        seq_t q1[65] = {0};          /* will hold degree 0 … 60 */

        for (idx_t i = 0; i < 65; ++i)
            temp[i] = a_half[i];

        for (idx_t i = 64; i >= d5; --i) {
            seq_t coef = temp[i];
            if (coef == 0) continue;
            idx_t shift = i - d5;
            q1[shift] = coef;
            for (idx_t j = 0; j <= d5; ++j) {
                temp[shift + j] -= coef * B[j];
            }
        }

        /* 3.  Divide the result by Φ_13(x) = 1 + x + … + x¹².  */
        const seq_t C[13] = {(seq_t)1, (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)1,
                             (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)1,
                             (seq_t)1, (seq_t)1, (seq_t)1};
        const idx_t d13 = 12;          /* deg Φ_13 = 12 */

        seq_t temp2[61];
        seq_t q2[61] = {0};           /* will hold degree 0 … 48 */

        for (idx_t i = 0; i < 61; ++i)
            temp2[i] = q1[i];

        for (idx_t i = 60; i >= d13; --i) {
            seq_t coef = temp2[i];
            if (coef == 0) continue;
            idx_t shift = i - d13;
            q2[shift] = coef;
            for (idx_t j = 0; j <= d13; ++j) {
                temp2[shift + j] -= coef * C[j];
            }
        }

        /* 4.  Store the 49 coefficients.  */
        for (idx_t i = 0; i < 49; ++i)
            coeff[i] = q2[i];

        initialized = true;
    }

    return (n < 49) ? coeff[(idx_t)n] : (seq_t)0;
}
