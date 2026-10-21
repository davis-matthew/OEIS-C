/*
 * A224138  Number of 7 X n 0..1 arrays with rows nondecreasing and
 *          antidiagonals unimodal.
 * https://oeis.org/A224138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in x*(128 + 1163*x - 4224*x^2 +
 *            1611*x^3 + 9957*x^4 - 14526*x^5 + 3960*x^6 + 4357*x^7 - 1401*x^8
 *            - 1818*x^9 + 659*x^10 + 353*x^11 - 155*x^12) / (1-x)^8 This
 *            expands to a(n) = Σ_{i=0}^{12} p_i * binom(n-1-i+7, 7) (for i ≤
 *            n-1), where the coefficients p_i are listed below
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 41, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    static const int coeffs[13] = {
        128, 1163, -4224, 1611, 9957, -14526,
        3960, 4357, -1401, -1818, 659, 353, -155
    };
    seq_t pos = 0;
    seq_t neg = 0;
    idx_t limit = n - 1; /* highest i to consider */
    for (idx_t i = 0; i < 13 && i <= limit; ++i) {
        idx_t m = limit - i;          /* n-1-i */
        idx_t k = m + 7;              /* choose k, 7 */
        seq_t b = binom(k, 7);        /* binom(k,7) */
        int c = coeffs[i];
        if (c > 0)
            pos += b * (seq_t)c;
        else
            neg += b * (seq_t)(-c);
    }
    return pos - neg;
}
