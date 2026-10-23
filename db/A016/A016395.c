/*
 * A016395  377th cyclotomic polynomial.
 * https://oeis.org/A016395
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   a(n) = coefficient of x^n in Φ_377(x) = (x^378 - x^377 - x + 1)
 *            / (1 - x^13 - x^29 + x^42)
 * Range:     i8 n <= 336, i16 n <= 336, i32 n <= 336, i64 n <= 336,
 *            i128 n <= 336, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static bool init = false;
    static seq_t coeff[337];   /* a(0) … a(336) */

    if (!init) {
        /* zero‐initialise the coefficient array */
        for (idx_t i = 0; i < 337; ++i) coeff[i] = (seq_t)0;

        /* numerator N(x) = x^378 - x^377 - x + 1 */
        seq_t temp[379];
        for (idx_t i = 0; i < 379; ++i) temp[i] = (seq_t)0;
        temp[0] = (seq_t)1;
        temp[1] = (seq_t)(-1);
        temp[377] = (seq_t)(-1);
        temp[378] = (seq_t)1;

        /* denominator D(x) = 1 - x^13 - x^29 + x^42 */
        seq_t D[43];
        for (idx_t i = 0; i < 43; ++i) D[i] = (seq_t)0;
        D[0] = (seq_t)1;
        D[13] = (seq_t)(-1);
        D[29] = (seq_t)(-1);
        D[42] = (seq_t)1;

        /* perform long division N(x) / D(x) */
        for (idx_t i = 378; i >= 42; --i) {
            seq_t coef = temp[i];
            if (coef != (seq_t)0) {
                coeff[i-42] = coef;
                for (idx_t j = 0; j < 43; ++j) {
                    temp[(i-42)+j] -= coef * D[j];
                }
            }
        }
        init = true;
    }

    if (n > 336) return (seq_t)0;
    return coeff[(size_t)n];
}
