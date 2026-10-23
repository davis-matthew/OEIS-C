/*
 * A199754  a(n) = (5*11^n+1)/2.
 * https://oeis.org/A199754
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*11^n + 1)/2 * a(0) = 3 * a(1) = 28 * a(n) = 11*a(n-1)
 *            - 5, n >= 1
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Compute 11^n as a seq_t value */
    seq_t pow11 = big_pow((seq_t)11, (seq_t)n);

    /* Avoid overflow by using integer arithmetic that works for all
     * sizes of seq_t.  Because 11 is odd, 11^n is odd for all n,
     * we can rewrite:
     *   a(n) = 5*(pow11/2) + 3
     * where pow11/2 is integer division by 2.
     */
    seq_t half = pow11 >> 1;            /* pow11 / 2   */
    seq_t res  = half * (seq_t)5;       /* 5 * (pow11/2) */
    if (pow11 & (seq_t)1)               /* always true, but kept generic */
        res += (seq_t)3;                /* +3 for odd pow11 */
    return res;
}
