/*
 * A025452  Number of partitions of n into 8 nonnegative cubes.
 * https://oeis.org/A025452
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Number of partitions of n into 8 nonnegative cubes
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function counts unordered multisets of 8 cubes (including 0³) that sum to n. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    const idx_t max_parts = 8;
    const size_t cols = (size_t)n + 1;
    const size_t rows = max_parts + 1;
    const size_t total = rows * cols;

    /* Allocate DP array (C++ uses new, C uses calloc) */
#ifdef __cplusplus
    seq_t *dp = new seq_t[total]();
#else
    seq_t *dp = (seq_t *)calloc(total, sizeof(seq_t));
#endif
    if (!dp) return 0;   /* allocation failure – should not happen for n ≤ 10000 */

    /* Base case: zero parts sum to zero */
    dp[0 * cols + 0] = 1;

    /* Process each positive cube value */
    for (idx_t k = 1; ; ++k) {
        idx_t cube = k * k * k;
        if (cube > n) break;

        /* Unbounded knapsack over sum and part count */
        for (idx_t s = cube; s <= n; ++s) {
            for (idx_t p = 1; p <= max_parts; ++p) {
                seq_t add = dp[(p - 1) * cols + (s - cube)];
                if (add) {
                    dp[p * cols + s] = dp[p * cols + s] + add;
                }
            }
        }
    }

    /* Sum over all part counts to allow padding with zeros. */
    seq_t result = 0;
    for (idx_t p = 0; p <= max_parts; ++p) {
        result = result + dp[p * cols + (size_t)n];
    }

    /* Free memory */
#ifdef __cplusplus
    delete[] dp;
#else
    free(dp);
#endif

    return result;
}
