/*
 * A097796  Number of partitions of n into perfect numbers.
 * https://oeis.org/A097796
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into perfect numbers (6, 28,
 *            496, 8128)
 * Range:     u8 n <= 4351, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Coins: perfect numbers ≤ 10000 */
static const idx_t perfects[] = {6, 28, 496, 8128};
static const size_t num_perfects = sizeof(perfects) / sizeof(perfects[0]);

seq_t a(idx_t n)
{
    /* Allocate DP array of size n+1, initialized to 0 */
    size_t len = (size_t)n + 1;
    seq_t *dp = (seq_t *)calloc(len, sizeof(seq_t));
    if (!dp) return 0; /* allocation failure; unlikely in test environment */

    dp[0] = 1; /* one way to make sum 0 */

    for (size_t i = 0; i < num_perfects; ++i)
    {
        idx_t coin = perfects[i];
        if ((idx_t)len <= coin) continue;
        for (idx_t j = coin; j <= (idx_t)n; ++j)
            dp[j] += dp[j - coin];
    }

    seq_t result = dp[(size_t)n];
    free(dp);
    return result;
}
