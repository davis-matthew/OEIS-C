/*
 * A035464  Number of partitions of n into parts 8k+4 or 8k+6.
 * https://oeis.org/A035464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into parts 8k+4 or 8k+6
 * Range:     u8 n <= 71, u16 n <= 177, u32 n <= 515, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation performed by a simple DP over the allowed part sizes. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static DP table up to the currently computed maximum n */
    static seq_t *dp = NULL;
    static idx_t cap = 0;

    idx_t len = n + 1;
    if (!dp || cap < len) {
        /* grow table if necessary */
        seq_t *newdp = (seq_t *)malloc(len * sizeof(seq_t));
        if (!newdp) return (seq_t)0;          /* out of memory – return zero */
        free(dp);
        dp = newdp;
        cap = len;
    }

    /* initialise dp[0]=1 and others to 0 */
    dp[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        dp[i] = (seq_t)0;

    /* iterate over allowed part sizes: 4,6,12,14,... (8k+4 or 8k+6) */
    for (idx_t part = 4; part <= n; part += 2) {
        if ((part & 7) == 4 || (part & 7) == 6) {
            for (idx_t i = part; i <= n; ++i)
                dp[i] = dp[i] + dp[i - part];
        }
    }

    return dp[n];
}
