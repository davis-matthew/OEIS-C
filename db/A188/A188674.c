/*
 * A188674  Stack polyominoes with square core.
 * https://oeis.org/A188674
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unimodal compositions of n in which the *
 *            number of parts equal to the maximum value equals that maximum
 *            value. * This is also the number of stack polyominoes of area n
 *            with a square core. * Computation is based on the
 *            generating‑function formula * G(x) = 1 + Σ_{m≥0} x^{(m+1)^2}...
 * Range:     u8 n <= 22, u16 n <= 51, u32 n <= 141, u64 n <= 439,
 *            u128 n <= 1000, big any n
 * Helpers:   _floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/_floor_isqrt_u64.h"

#ifdef __cplusplus
#include <new>
#endif

seq_t a(idx_t n)
{
    /* a(0) = 1 by definition of the empty polyomino / empty composition */
    if (n == 0) return 1U;

    /* maximum core side length m with m² ≤ n */
    idx_t maxM = _floor_isqrt_u64(n);

    /* Allocate array for partition counts.  We need n+1 entries. */
    seq_t *dp;
#if defined(__cplusplus)
    dp = new seq_t[(size_t)(n + 1)];
#else
    dp = (seq_t*)calloc((size_t)(n + 1), sizeof(seq_t));
#endif
    if (!dp) return 0;           /* allocation failed – unlikely in tests */

    dp[0] = 1;  /* the empty partition */

    seq_t result = 0;

    for (idx_t m = 1; m <= maxM; ++m) {
        idx_t k = m - 1;                     /* new part size to allow */

        if (k > 0) {
            /* Unbounded knapsack update for part size k */
            for (idx_t j = k; j <= n; ++j) {
                dp[j] = dp[j] + dp[j - k];
            }
        }

        idx_t R = n - m * m;                /* area left after the core */

        /* Convolution: sum_{p=0}^{R} dp[p] * dp[R-p] */
        seq_t sum = 0;
        /* type cast to size_t for array indexing */
        for (size_t p = 0; p <= (size_t)R; ++p) {
            sum = sum + dp[p] * dp[R - p];
        }
        result = result + sum;
    }

#if defined(__cplusplus)
    delete[] dp;
#else
    free(dp);
#endif
    return result;
}
