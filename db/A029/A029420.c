/*
 * A029420  Expansion of 1/((1-x^6)*(1-x^7)*(1-x^10)*(1-x^11)).
 * https://oeis.org/A029420
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = number of partitions of n into parts 6, 7, 10, and 11
 * Range:     u8 n <= 175, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *dp = NULL;
    static idx_t dp_size = 0;   /* current size of dp (max_n+1) */

    /* if requested index is beyond current cache, enlarge and recompute */
    if (n + 1 > dp_size)
    {
        idx_t new_size = n + 1;
        size_t bytes = (size_t)new_size * sizeof(seq_t);

        /* allocate new array */
        seq_t *new_dp = (seq_t *)malloc(bytes);
        if (!new_dp) return 0;  /* allocation failed, return 0 as safe fallback */

        /* initialise to 0 and set base case */
        for (idx_t i = 0; i < new_size; ++i) new_dp[i] = 0;
        new_dp[0] = 1;   /* one way to partition 0 */

        /* dynamic programming over the four allowed part sizes */
        const idx_t parts[4] = {6, 7, 10, 11};
        for (int p = 0; p < 4; ++p)
        {
            idx_t coin = parts[p];
            for (idx_t i = coin; i < new_size; ++i)
                new_dp[i] += new_dp[i - coin];
        }

        /* replace old cache */
        if (dp != NULL) free(dp);
        dp = new_dp;
        dp_size = new_size;
    }

    return dp[n];
}
