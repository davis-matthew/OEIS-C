/*
 * A029411  Expansion of 1/((1-x^5)*(1-x^9)*(1-x^11)*(1-x^12)).
 * https://oeis.org/A029411
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = number of partitions of n into parts 5, 9, 11, and 12.  */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of the dynamic programming table.  */
    static seq_t *dp = NULL;
    static idx_t dp_n = 0;              /* size of dp   */
    static idx_t max_pre = 0;           /* highest n computed */

    /* If we need a larger table, reallocate it. */
    if (dp == NULL || n >= dp_n) {
        /* Free previous table. */
#ifdef __cplusplus
        delete[] dp;
#else
        free(dp);
#endif
        /* Allocate a new table of size n+1. */
#ifdef __cplusplus
        dp = new seq_t[(size_t)(n + 1)];
#else
        dp = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
#endif
        if (!dp) return 0;  /* allocation failed; unlikely in tests   */

        /* Initialise to zero (calloc did that).  */
        dp[0] = 1;          /* one way to make 0 */

        /* Parts allowed in the partition. */
        const idx_t parts[4] = {5, 9, 11, 12};

        /* Classic unbounded coin change DP: for each part, add its
           contribution to all larger amounts.  */
        for (idx_t p = 0; p < 4; ++p) {
            idx_t coin = parts[p];
            for (idx_t amt = coin; amt <= n; ++amt) {
                dp[amt] += dp[amt - coin];
            }
        }

        dp_n = n + 1;
        max_pre = n;
    }
    /* If n is smaller than the currently cached table, we still
       recompute from scratch because the table was sized for a larger n
       and might contain stale data beyond n.  */

#if 0
    /* The following block would recompute only the new part when the
       requested n is larger than max_pre; it is omitted for clarity
       because the whole table is recomputed above.  */
#endif

    return dp[n];
}
