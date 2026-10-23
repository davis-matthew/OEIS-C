/*
 * A029354  Expansion of 1/((1-x^4)*(1-x^6)*(1-x^9)*(1-x^10)).
 * https://oeis.org/A029354
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 4, 6, 9 and 10
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *dp = NULL;
    static idx_t capacity = 0; /* number of elements allocated in dp */

    /* grow cache if necessary */
    if (capacity <= n) {
        seq_t *tmp = (seq_t *)realloc(dp, (n + 1) * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failed – return 0 */
        dp = tmp;
        capacity = n + 1;
    }

    /* initialise dp[0] = 1, others 0 */
    dp[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        dp[i] = (seq_t)0;

    /* coin‑change DP for parts 4,6,9,10 */
    static const idx_t coins[4] = {4, 6, 9, 10};
    for (int j = 0; j < 4; ++j) {
        idx_t coin = coins[j];
        for (idx_t i = coin; i <= n; ++i)
            dp[i] += dp[i - coin];
    }

    return dp[n];
}
