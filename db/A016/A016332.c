/*
 * A016332  133rd cyclotomic polynomial.
 * https://oeis.org/A016332
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   a(n) = coefficient of x^n in the cyclotomic polynomial Φ₁₃₃(x)
 * Range:     i8 n <= 108, i16 n <= 108, i32 n <= 108, i64 n <= 108,
 *            i128 n <= 108, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t coeff[109];
    static bool init = false;
    if (!init) {
        /* Build Φ₇(x) (coefficients 0..6 all 1) */
        seq_t poly7[7];
        for (idx_t i = 0; i < 7; ++i) poly7[i] = 1;

        /* Build Φ₁₉(x) (coefficients 0..18 all 1) */
        seq_t poly19[19];
        for (idx_t i = 0; i < 19; ++i) poly19[i] = 1;

        /* Product Φ₇(x) * Φ₁₉(x) -> degree 24 */
        seq_t prod[25];
        for (idx_t i = 0; i < 25; ++i) prod[i] = 0;
        for (idx_t i = 0; i < 7; ++i)
            for (idx_t j = 0; j < 19; ++j)
                prod[i + j] += poly7[i] * poly19[j];

        /* Multiply by (x - 1) -> D(x) = (x - 1)·Φ₇·Φ₁₉ .  Degree 25. */
        seq_t D[26];
        for (idx_t i = 0; i < 26; ++i) D[i] = 0;
        for (idx_t i = 0; i < 25; ++i) {
            D[i]     -= prod[i];   /* times -1 */
            D[i + 1] += prod[i];   /* times  x */
        }

        /* Numerator N(x) = x¹³³ - 1 -> degree 133. */
        seq_t N[134];
        for (idx_t i = 0; i < 134; ++i) N[i] = 0;
        N[0]     = -1;
        N[133]   =  1;

        /* Long division N / D to obtain Φ₁₃₃(x) -> Q[0..108] */
        seq_t Q[109];
        for (idx_t i = 0; i < 109; ++i) Q[i] = 0;

        for (idx_t i = 133;; --i) {
            seq_t q_coeff = N[i];          /* D[25] == 1, so division is trivial. */
            Q[i - 25] = q_coeff;
            if (q_coeff != 0) {
                for (idx_t j = 0; j < 26; ++j)
                    N[i - 25 + j] -= q_coeff * D[j];
            }
            if (i == 25) break;
        }

        for (idx_t i = 0; i < 109; ++i) coeff[i] = Q[i];
        init = true;
    }
    if (n < 109) return coeff[n];
    return (seq_t)0;
}
