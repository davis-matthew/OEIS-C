/*
 * A035446  Number of partitions of n into parts 8k or 8k+6.
 * https://oeis.org/A035446
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 85, u16 n <= 205, u32 n <= 569, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of partitions of n into parts 8k or 8k+6. */
#include <stdlib.h>
#include <string.h>

/* External types provided by the build system:
   seq_t   – unsigned integer type (up to 128 bits or arbitrary precision)
   idx_t   – unsigned integer type (uint64_t)   */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* Allocate DP array */
    size_t N = (size_t)n + 1;
    seq_t *dp = (seq_t *)malloc(N * sizeof(seq_t));
    if (!dp) return (seq_t)0;          /* allocation failed */
    memset(dp, 0, N * sizeof(seq_t));
    dp[0] = (seq_t)1;                  /* one way to partition 0 */

    /* Parts of the form 8k (k ≥ 1) */
    for (idx_t k = 1; (idx_t)(8ULL * k) <= n; ++k) {
        idx_t part = (idx_t)(8ULL * k);
        for (idx_t i = part; i <= n; ++i) {
            dp[(size_t)i] += dp[(size_t)(i - part)];
        }
    }

    /* Parts of the form 8k+6 (k ≥ 0) */
    for (idx_t k = 0; (idx_t)(8ULL * k + 6ULL) <= n; ++k) {
        idx_t part = (idx_t)(8ULL * k + 6ULL);
        for (idx_t i = part; i <= n; ++i) {
            dp[(size_t)i] += dp[(size_t)(i - part)];
        }
    }

    seq_t result = dp[(size_t)n];
    free(dp);
    return result;
}
