/*
 * A027630  Molien series for Gamma_3(2).
 * https://oeis.org/A027630
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0}^{10} c[k] * binom(n-k+7, 7) Coefficients after
 *            expanding (1 - x⁴)(1 + 7x + 43x² + 154x³ + 43x⁴ + 7x⁵ + x⁶) are
 *            c[0] = 1 c[1] = 7 c[2] = 43 c[3] = 154 c[4] = 42 c[5] = 0 c[6] =
 *            -42 c[7] = -154 c[8] = -43 c[9] = -7 c[10] = -1 a(0)=1, a(1)=15,
 *            a(2)=135, a(3)=870, …
 * Range:     u8 n <= 0, u16 n <= 6, u32 n <= 38, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binom_n7
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom_n7.h"

seq_t a(idx_t n)
{
    /* coefficients of the numerator polynomial */
    static const int32_t c[11] =
    {  1,   7,  43, 154,  42,   0, -42, -154, -43,  -7, -1 };

    seq_t result = 0;
    for (int k = 0; k <= 10; ++k)
    {
        if (n < (idx_t)k)          /* binomial is zero for negative argument */
            continue;

        idx_t m = n - (idx_t)k + 7;   /* m >= 7 */
        seq_t bin = binom_n7(m);

        if (c[k] >= 0)
            result += (seq_t)c[k] * bin;
        else
            result -= (seq_t)(-c[k]) * bin;
    }
    return result;
}
