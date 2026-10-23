/*
 * A029358  Expansion of 1/((1-x^4)*(1-x^6)*(1-x^11)*(1-x^12)).
 * https://oeis.org/A029358
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 4, 6, 11, and 12
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Computes a(n) for the OEIS sequence A029358.
 * The function is compliant with both C11 and C++17.
 * It uses a simple dynamic‑programming approach
 * that works in O(n) time and O(n) memory.
 */

seq_t a(idx_t n)
{
    /* Allocate an array of size n+1. In C++ use new[]
       to construct the elements; in C use calloc to zero‑initialize. */
#ifdef __cplusplus
    seq_t *dp = new seq_t[(size_t)(n + 1)]();   /* default constructed, zero‑init */
#else
    seq_t *dp = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
#endif
    if (dp == NULL) {
        /* allocation failed – return zero as a safe fallback */
        return (seq_t)0;
    }

    /* number of partitions of zero is one (empty sum) */
    dp[0] = (seq_t)1;

    /* Coins (allowed parts) */
    const idx_t parts[] = {4, 6, 11, 12};
    const size_t part_cnt = sizeof(parts) / sizeof(parts[0]);

    /* Coin‑change DP: iterate over each part and update dp in forward order. */
    for (size_t pi = 0; pi < part_cnt; ++pi) {
        idx_t coin = parts[pi];
        for (idx_t i = coin; i <= n; ++i) {
            dp[i] = dp[i] + dp[i - coin];
        }
    }

    seq_t result = dp[n];

    /* Free the allocated memory. */
#ifdef __cplusplus
    delete[] dp;
#else
    free(dp);
#endif

    return result;
}
