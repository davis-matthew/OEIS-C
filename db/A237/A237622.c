/*
 * A237622  Interpolation polynomial through n points (0,1), (1,1), ...,
 *          (n-2,1) and (n-1,n) evaluated at 2n, a(0)=1.
 * https://oeis.org/A237622
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + (n-1) * binomial(2*n, n+1) (n ≥ 0) For n < 2, this
 *            reduces to a(0)=1, a(1)=1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* a(0) = 1, a(1) = 1 */
    if (n <= 1)
        return (seq_t)1;

    seq_t res = 1;                      /* res = C(2n, n+1) */
    for (idx_t k = 1; k <= n - 1; ++k) {
        res *= (seq_t)(n + 1 + k);       /* multiply by numerator term */
        res /= (seq_t)k;                /* exact division by denominator term */
    }

    res *= (seq_t)(n - 1);               /* multiply by (n-1) */
    return res + 1;                     /* add the constant 1 */
}
