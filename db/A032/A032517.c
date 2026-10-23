/*
 * A032517  Sum of the integer part of 9/2-th roots of integers less than n.
 * https://oeis.org/A032517
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1}^{n} floor(k^(2/9))
 * Range:     u8 n <= 0, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    /* sum up to n; for n==0 the loop body is skipped and sum==0 */
    for (idx_t k = 1; k <= n; ++k)
    {
        /* In the sequence definition we need floor((k)^(2/9)).
           Since (k)^(2/9) = sqrt( k^2 )^(1/9),
           we can compute an integer root r that satisfies
           r^9 <= k^2 < (r+1)^9.  The loop below finds the
           largest r with r^9 <= k^2.  With the typical
           values of n (<100 000) r is very small (≤20),
           so a simple incremental search is fast.  */
        seq_t kk = (seq_t)k * (seq_t)k;   /* k^2   */
        seq_t m = 1;
        for (;; ++m)
        {
            /* compute m^9 */
            seq_t p = 1;
            for (int i = 0; i < 9; ++i) p *= m;
            if (p > kk) break;
        }
        /* m is the first value with m^9 > kk, so floor root is m-1 */
        sum += (m - 1);
    }
    return sum;
}
