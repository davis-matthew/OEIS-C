/*
 * A214772  Number of partitions of n into parts 6, 9 or 20.
 * https://oeis.org/A214772
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 6, 9 or 20
 * Range:     u8 n <= 701, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Return the number of partitions of n into parts 6, 9 or 20. */
seq_t a(idx_t n)
{
    /* Static storage for dynamic programming array.
       It is resized on the first call or if a larger n is requested. */
    static seq_t *dp      = NULL;   /* array of counts */
    static idx_t  dp_size = 0;      /* current allocated size (n+1) */

    /* Allocate or enlarge the array if needed. */
    if (dp_size <= n) {
        dp = (seq_t *)realloc(dp, sizeof(seq_t) * ((size_t)(n + 1)));
        dp_size = n + 1;
    }

    /* Initialise: only amount 0 has one representation. */
    dp[0] = (seq_t)1;
    for (idx_t i = 1; i < dp_size; ++i) dp[i] = 0;

    /* Classic coin‑change dynamic programming
       (order of parts does not matter). */
    const idx_t coins[3] = {6, 9, 20};
    for (int c = 0; c < 3; ++c) {
        idx_t coin = coins[c];
        for (idx_t i = coin; i < dp_size; ++i) {
            dp[i] += dp[i - coin];
        }
    }

    return dp[n];
}
