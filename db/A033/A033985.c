/*
 * A033985  Number of partitions of n into two or more distinct nonzero
 *          squares.
 * https://oeis.org/A033985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into distinct nonzero squares
 *            using at least two squares. Computed by dynamic programming on
 *            the generating function ∏_{k≥1}(1 + x^{k²}) and subtracting the
 *            single-square case
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   _floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/_floor_isqrt_u64.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    /* Allocate DP array of size n+1.  Use size_t for allocation to avoid
       overflow for the very small n values considered. */
    seq_t *dp = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (dp == NULL)
        return (seq_t)0;  /* allocation failure */

    dp[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        dp[i] = (seq_t)0;

    /* Count partitions into distinct squares (any number of parts). */
    idx_t max_k = _floor_isqrt_u64(n);
    for (idx_t k = 1; k <= max_k; ++k) {
        idx_t sq = k * k;
        for (idx_t i = n; i >= sq; --i) {
            dp[i] += dp[i - sq];
        }
    }

    seq_t result = dp[n];
    free(dp);

    /* Subtract the single-square partition if n itself is a perfect square. */
    idx_t sqroot = _floor_isqrt_u64(n);
    if (sqroot * sqroot == n)
        result -= (seq_t)1;

    return result;
}
