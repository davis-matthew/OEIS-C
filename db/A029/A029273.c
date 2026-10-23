/*
 * A029273  Expansion of 1/((1-x^3)*(1-x^5)*(1-x^6)*(1-x^8)).
 * https://oeis.org/A029273
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 92, u16 n <= 645, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A029273: partitions of n into parts 3,5,6,8 via dynamic programming */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate array of size n+1.  Use malloc so it works for both plain
       integer types and possible big‑integer classes, then explicitly
       zero the elements. */
    seq_t *dp = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!dp) return 0;                 /* out of memory */
    for (idx_t i = 0; i <= n; ++i) dp[i] = 0;
    dp[0] = 1;                         /* one way to make sum 0 */

    /* The allowed parts.  They are constants; the order is irrelevant. */
    static const idx_t parts[4] = {3, 5, 6, 8};

    /* Standard coin‑change DP for unlimited copies of each part. */
    for (int k = 0; k < 4; ++k)
    {
        idx_t p = parts[k];
        for (idx_t i = p; i <= n; ++i)
        {
            dp[i] += dp[i - p];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
