/*
 * A029314  Expansion of 1/((1-x^3)*(1-x^7)*(1-x^10)*(1-x^12)).
 * https://oeis.org/A029314
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 3, 7, 10, 12
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed via dynamic programming: dp[0] = 1; for each part p, dp[i] += dp[i-p] */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to reuse computed DP array for subsequent calls */
    static seq_t *dp = NULL;
    static idx_t dp_size = 0;
    const idx_t parts[4] = {3, 7, 10, 12};
    int i, p;

    /* extend the cache if needed */
    if ((idx_t)(n + 1) > dp_size) {
        seq_t *newdp = (seq_t *)realloc(dp, (n + 1) * sizeof(seq_t));
        if (!newdp) return 0;          /* allocation failed */
        dp = newdp;
        dp_size = n + 1;
    }

    /* zero out the relevant portion of the DP array */
    for (i = 0; i <= (int)n; ++i)
        dp[i] = 0;
    dp[0] = 1;

    /* update DP for each allowed part size */
    for (p = 0; p < 4; ++p) {
        idx_t part = parts[p];
        for (i = (int)part; i <= (int)n; ++i) {
            dp[i] += dp[i - part];
        }
    }

    return dp[(int)n];
}
