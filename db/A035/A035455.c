/*
 * A035455  Number of partitions of n into parts 8k+2 or 8k+4.
 * https://oeis.org/A035455
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into parts 8k+2 or 8k+4
 * Range:     u8 n <= 51, u16 n <= 143, u32 n <= 449, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation using dynamic programming (unbounded coin change). */
#include <stdlib.h>

seq_t a(idx_t n) {
    /* Allocate DP array for 0..n */
    seq_t *dp = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    if (!dp) return 0; /* fallback if allocation fails */

    /* Initialize */
    for (idx_t i = 0; i <= n; ++i) dp[i] = 0;
    dp[0] = 1;                           /* empty partition */

    /* For each allowed part size, update DP */
    for (idx_t k = 0;; ++k) {
        idx_t s1 = 8 * k + 2;             /* part 8k+2 */
        if (s1 > n) break;               /* beyond n, stop */
        for (idx_t i = s1; i <= n; ++i)
            dp[i] += dp[i - s1];

        idx_t s2 = 8 * k + 4;             /* part 8k+4 */
        if (s2 > n) continue;            /* handled above */
        for (idx_t i = s2; i <= n; ++i)
            dp[i] += dp[i - s2];
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
