/*
 * A029045  Expansion of 1/((1-x)*(1-x^3)*(1-x^6)*(1-x^7)).
 * https://oeis.org/A029045
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A029045: number of partitions of n into parts 1, 3, 6 and 7.
   Computed by a standard unbounded coin‐change DP recurrence. */

#include <stdlib.h>  /* malloc, realloc, abort */

seq_t a(idx_t n)
{
    /* allocate/store the DP table once and reuse it for future calls. */
    static seq_t *dp = NULL;
    static idx_t dp_size = 0;          /* current allocated slots */

    /* enlarge the table if necessary (allocate n+1 slots). */
    if (dp_size < n + 1) {
        seq_t *tmp = (seq_t *)realloc(dp, (n + 1) * sizeof(seq_t));
        if (!tmp) abort();
        dp   = tmp;
        dp_size = n + 1;
    }

    /* clear used part of the table.  dp[0] will be set to 1 below. */
    for (idx_t i = 1; i <= n; ++i)
        dp[i] = 0;
    dp[0] = 1;

    /* unbounded coin change DP over the allowed part sizes. */
    const idx_t parts[4] = {1, 3, 6, 7};
    for (int k = 0; k < 4; ++k) {
        idx_t coin = parts[k];
        for (idx_t i = coin; i <= n; ++i)
            dp[i] += dp[i - coin];
    }

    return dp[n];
}
