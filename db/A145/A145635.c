/*
 * A145635  Numerator of the polynomial A_l(x) = sum_{d=1..l-1} x^(l-d)/d for
 *          index l=2n+1 evaluated at x=14.
 * https://oeis.org/A145635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = numerator of sum_{d=1}^{2n} 14^(2n+1-d)/d in lowest terms
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* base value for n = 1 (l = 3) */
    if (n == 1)
        return 203;

    /* target l = 2n + 1 */
    idx_t l = (idx_t)(2 * n + 1);

    /* Maintain the reduced fraction N/D = S_l */
    seq_t N = 203;    /* numerator for l = 3 */
    seq_t D = 1;      /* denominator for l = 3 */

    /* Build up l to the target value using the recurrence
       S_{l+1} = 14 * S_l + 14/(l)  */
    for (idx_t cur = 4; cur <= l; ++cur)
    {
        idx_t prev = cur - 1;                /* previous l value */
        seq_t temp = N * (seq_t)prev + D;    /* N*prev + D      */
        seq_t new_num = temp * (seq_t)14;    /* 14 * (N*prev + D) */
        seq_t new_den = D * (seq_t)prev;     /* D * prev       */

        /* reduce the new fraction */
        seq_t g = gcd(new_num, new_den);
        N = new_num / g;
        D = new_den / g;
    }

    return N;
}
