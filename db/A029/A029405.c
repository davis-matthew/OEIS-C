/*
 * A029405  Expansion of 1/((1-x^5)*(1-x^8)*(1-x^9)*(1-x^12)).
 * https://oeis.org/A029405
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 5, 8, 9, and 12
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *dp = NULL;
    static idx_t cap = 0;
    const idx_t parts[4] = {5, 8, 9, 12};
    const idx_t need = n + 1;

    /* Allocate or reallocate the DP array if necessary. */
    if (need > cap) {
        free(dp);
        dp = (seq_t *)malloc((size_t)need * sizeof(seq_t));
        if (!dp) return 0;                /* Allocation failure, unlikely. */
        for (idx_t i = 0; i < need; ++i) dp[i] = 0;
        cap = need;
    } else {
        for (idx_t i = 0; i < need; ++i) dp[i] = 0;
    }

    dp[0] = 1;
    for (int p = 0; p < 4; ++p) {
        idx_t coin = parts[p];
        for (idx_t i = coin; i < need; ++i) {
            dp[i] += dp[i - coin];
        }
    }
    return dp[n];
}
