/*
 * A029185  Expansion of 1/((1-x^2)*(1-x^4)*(1-x^7)*(1-x^12)).
 * https://oeis.org/A029185
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 2, 4, 7, and 12
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of values 0 .. max_n computed so far */
    static seq_t *dp   = NULL;   /* array of seq_t, dp[0] = 1 */
    static idx_t  size = 0;      /* allocated size (max index + 1) */

    if (n >= size) {
        /* need to recompute up to new maximum n */
        idx_t new_size = n + 1;
        seq_t *new_dp = (seq_t *)calloc(new_size, sizeof(seq_t));
        if (!new_dp) abort();   /* allocation failure */

        /* base case */
        new_dp[0] = 1;

        /* coin denominations */
        static const idx_t coins[4] = {2, 4, 7, 12};

        /* dynamic programming: for each coin, update the table */
        for (int k = 0; k < 4; ++k) {
            idx_t coin = coins[k];
            for (idx_t i = coin; i <= n; ++i) {
                new_dp[i] += new_dp[i - coin];
            }
        }

        /* replace old table */
        free(dp);
        dp   = new_dp;
        size = new_size;
    }

    return dp[n];
}
