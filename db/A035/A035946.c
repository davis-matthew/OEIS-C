/*
 * A035946  Number of partitions in parts not of the form 11k, 11k+3 or 11k-3.
 *          Also number of partitions with at most 2 parts of size 1 and
 *          differences between parts at distance 4 are greater than 1.
 * https://oeis.org/A035946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts not congruent to 0,
 *            3, or 8 modulo 11
 * Range:     u8 n <= 19, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses an O(n²) dynamic‑programming routine
   that works for any idx_t / seq_t pair, without relying on
   external utilities.  */
/* It keeps a static table that is expanded to the requested
   index and recomputed from scratch whenever a larger index is
   queried.  The recurrence is
         dp[0] = 1
         for each allowed part p:
             for s = p .. n:
                 dp[s] += dp[s - p]   */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* The OEIS offset is 1; for n < 1 the sequence is undefined. */
    if (n < 1)
        return (seq_t)0;

    /* Static storage for the partition table. */
    static seq_t *dp = NULL;   /* dp[0..max_n] */
    static idx_t max_n = 0;    /* largest index currently computed */

    /* Allocate or expand the table to accommodate n. */
    if (dp == NULL || n > max_n) {
        seq_t *newdp = (seq_t *)realloc(dp, sizeof(seq_t) * (n + 1));
        if (newdp == NULL)
            return (seq_t)0;           /* allocation failure – graceful exit */
        dp = newdp;

        /* Reset the table up to n. */
        dp[0] = (seq_t)1;
        for (idx_t s = 1; s <= n; ++s)
            dp[s] = (seq_t)0;

        /* DP recurrence over allowed parts. */
        for (idx_t p = 1; p <= n; ++p) {
            int r = (int)(p % 11);
            if (r == 0 || r == 3 || r == 8)
                continue;               /* forbidden part */
            for (idx_t s = p; s <= n; ++s)
                dp[s] = dp[s] + dp[s - p];
        }
        max_n = n;
    }

    /* Return the pre‑computed value. */
    return dp[n];
}
