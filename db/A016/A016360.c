/*
 * A016360  221st cyclotomic polynomial.
 * https://oeis.org/A016360
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   a(n) = coefficient of x^n in the 221st cyclotomic polynomial
 *            Φ₂₂₁(x). Recurrence: Let I(m) = 1 if m≡0 (mod 17), else 0. Then
 *            for 0 ≤ m ≤ 192 a(m) = I(m) – ∑_{i=1}^{12} a(m – i) (with a(k)=0
 *            for k<0). For m > 192, a(m)=0 (degree φ(221)=192)
 * Range:     i8 n <= 192, i16 n <= 192, i32 n <= 192, i64 n <= 192,
 *            i128 n <= 192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* φ(221) = φ(13·17) = 12·16 = 192 */
    static seq_t coeff[193];
    static bool initialized = false;

    if (!initialized) {
        /* compute all coefficients once */
        for (idx_t m = 0; m <= 192; ++m) {
            seq_t sum = 0;
            for (int i = 1; i <= 12; ++i) {
                if (m >= (idx_t)i)
                    sum += coeff[m - i];
            }
            seq_t indicator = (m % 17 == 0) ? 1 : 0;
            coeff[m] = indicator - sum;
        }
        initialized = true;
    }

    if (n > 192)
        return 0;
    return coeff[n];
}
