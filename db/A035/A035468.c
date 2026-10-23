/*
 * A035468  Number of partitions of n into parts 8k+6 or 8k+7.
 * https://oeis.org/A035468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 94, u16 n <= 217, u32 n <= 584, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A035468: Number of partitions of n into parts 8k+6 or 8k+7
   Computed by dynamic programming. */
/* The algorithm uses the standard partition DP:
   dp[0] = 1; for each allowed part p:
   for (i = p; i <= n; ++i) dp[i] += dp[i - p];   */

#include <stdlib.h>
#if defined(__cplusplus)
#include <cstddef>
#endif

seq_t a(idx_t n) {
    /* Allocate or extend the DP array as needed. */
    static seq_t *dp = NULL;
    static size_t dp_len = 0;          /* number of elements stored in dp (dp_len >= 1) */

    /* Ensure dp[0] = 1 and other elements are initialized to 0 */
    if (n + 1 > dp_len) {
        /* Reallocate to size n + 1 */
        seq_t *tmp = (seq_t *)realloc(dp, sizeof(seq_t) * ((size_t)n + 1));
        if (!tmp) { /* allocation failure; return zero */
            return (seq_t)0;
        }
        dp = tmp;
        /* Initialize all entries up to n */
        dp[0] = (seq_t)1;
        for (size_t i = 1; i <= (size_t)n; ++i) {
            dp[i] = (seq_t)0;
        }
        dp_len = (size_t)n + 1;
    }

    /* Recompute dp values up to n using the allowed parts 8k+6 and 8k+7.
       The computation is idempotent: if n has not increased since the
       last call, the DP is already correct. */
    for (idx_t k = 0;; ++k) {
        idx_t p6 = 8 * k + 6;
        if (p6 > n) break;
        /* Update dp for part p6 */
        for (idx_t i = p6; i <= n; ++i) {
            dp[i] = dp[i] + dp[i - p6];
        }
        idx_t p7 = 8 * k + 7;
        if (p7 > n) continue;
        /* Update dp for part p7 */
        for (idx_t i = p7; i <= n; ++i) {
            dp[i] = dp[i] + dp[i - p7];
        }
    }

    return dp[n];
}
