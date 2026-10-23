/*
 * A025813  Expansion of 1/((1-x^2)*(1-x^6)*(1-x^7)).
 * https://oeis.org/A025813
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute sequence A025813: number of partitions of n into parts 2, 6, and 7.
   Formula: a(n) = ⌊(n² + 15n + 7(n+8)(-1)^n + 119)/168⌋
   The implementation uses a dynamic programming table that grows as needed
   and recomputes when a larger n is requested.  It works for any integer
   type seq_t and idx_t provided by the build. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache the computed values */
    static seq_t *dp = NULL;
    static idx_t dp_size = 0;      /* number of entries in dp */
    static idx_t computed_max = 0; /* largest n for which dp[0..n] are correct */

    /* Grow the cache if necessary */
    if (n + 1 > dp_size) {
        seq_t *newdp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        if (!newdp)   /* allocation failed – return 0 as a safe fallback */
            return (seq_t)0;
        if (dp) free(dp);
        dp = newdp;
        dp_size = n + 1;
    }

    /* Recompute the table if we need a value beyond the previous maximum.
       The condition is >= so that n == 0 triggers the initial computation. */
    if (n >= computed_max) {
        /* Initialize base case and clear the rest of the table up to n */
        dp[0] = 1;
        for (idx_t i = 1; i <= n; ++i)
            dp[i] = 0;

        /* Standard unbounded knapsack DP for parts 2, 6, and 7 */
        const idx_t coins[3] = {2, 6, 7};
        for (int c = 0; c < 3; ++c) {
            idx_t p = coins[c];
            for (idx_t i = p; i <= n; ++i) {
                dp[i] += dp[i - p];
            }
        }
        computed_max = n;
    }

    return dp[n];
}
