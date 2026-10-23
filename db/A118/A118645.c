/*
 * A118645  Number of binary strings of length n such that there exist three
 *          consecutive digits where at least two of them are 1's.
 * https://oeis.org/A118645
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*2^(n-3) + a(n-1) + a(n-3) initial values: a(0)=0,
 *            a(1)=0, a(2)=1 This computes the number of binary strings of
 *            length n that contain a window of three consecutive digits with
 *            at least two 1's
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* allocate a temporary dp array large enough for the requested n
       (n <= 10000 is typical in tests). */
    seq_t *dp = (seq_t*)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!dp) return (seq_t)0;   /* out of memory, return 0 */

    for (idx_t i = 0; i <= n; ++i)
    {
        if (i == 0 || i == 1)
            dp[i] = (seq_t)0;
        else if (i == 2)
            dp[i] = (seq_t)1;
        else
        {
            seq_t pow2_val = pow2(i - 3);   /* 2^(i-3) */
            seq_t term = pow2_val * (seq_t)3;
            dp[i] = dp[i - 1] + dp[i - 3] + term;
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
