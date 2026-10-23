/*
 * A025457  Number of partitions of n into 4 positive cubes.
 * https://oeis.org/A025457
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into 4 positive cubes
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is computed by dynamic programming over the cube values. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate table dp[sum][k] for 0 ≤ sum ≤ n, 0 ≤ k ≤ 4. */
    size_t cap = (size_t)(n + 1) * 5ULL;
    seq_t *dp = (seq_t*)calloc(cap, sizeof(seq_t));
    if (!dp) return 0;                  /* allocation failed: return 0 */

    /* Base case: one way to sum to 0 with 0 cubes. */
    dp[0] = 1;                           /* dp[0][0] = 1 */

    /* Iterate over all cube values ≤ n. */
    for (idx_t k = 1; (k * k * k) <= n; ++k) {
        idx_t cube = k * k * k;
        for (idx_t sum = cube; sum <= n; ++sum) {
            size_t cur = (size_t)sum * 5ULL;
            for (idx_t use = 1; use <= 4; ++use) {
                size_t prev = (size_t)(sum - cube) * 5ULL + (use - 1);
                seq_t add = dp[prev];
                if (add) dp[cur + use] += add;
            }
        }
    }

    seq_t result = dp[(size_t)n * 5ULL + 4];    /* dp[n][4] */
    free(dp);
    return result;
}
