/*
 * A029085  Expansion of 1/((1-x)*(1-x^5)*(1-x^6)*(1-x^7)).
 * https://oeis.org/A029085
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the number of partitions of n into parts 1, 5, 6 and 7.
 *            It is computed by the classic coin‑change DP:
 * Range:     u8 n <= 59, u16 n <= 425, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   a(0) = 1
    a(n) = Σ_{c∈{1,5,6,7}} a(n–c)  for n≥1, with a(d)=0 for d<0. */

#include <stdlib.h>   /* malloc, free */

seq_t a(idx_t n)
{
    /* Static cache: dp[i] = a(i) for 0 ≤ i < cache_size. */
    static seq_t *dp = NULL;
    static idx_t cache_size = 0;   /* number of elements stored */

    /* If we don't have all values up to n, rebuild the table. */
    if (n >= cache_size) {
        idx_t new_size = n + 1;                     /* array length */
        /* Allocate a new table.  Use size_t for the allocation size. */
        seq_t *new_dp = (seq_t *)malloc((size_t)new_size * sizeof(seq_t));
        if (!new_dp) return (seq_t)0;               /* out of memory */

        /* Initialize.  a(0)=1, the rest start at 0. */
        new_dp[0] = (seq_t)1;
        for (idx_t i = 1; i < new_size; ++i) new_dp[i] = (seq_t)0;

        /* The coin denominations. */
        const idx_t COINS[4] = {1, 5, 6, 7};

        /* Classic DP: for each coin, add its contribution to higher indices. */
        for (int c = 0; c < 4; ++c) {
            idx_t coin = COINS[c];
            for (idx_t i = coin; i < new_size; ++i) {
                new_dp[i] += new_dp[i - coin];
            }
        }

        /* Replace the old table. */
        if (dp) free(dp);
        dp = new_dp;
        cache_size = new_size;
    }

    return dp[n];
}
