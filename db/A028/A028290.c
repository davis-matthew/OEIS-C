/*
 * A028290  Expansion of 1/((1-x)(1-x^2)(1-x^3)(1-x^5)(1-x^8)).
 * https://oeis.org/A028290
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = number of partitions of n into parts 1, 2, 3, 5, 8, i.e.
 *            coefficient of x^n in 1/((1‑x)(1‑x²)(1‑x³)(1‑x⁵)(1‑x⁸))
 * Range:     u8 n <= 25, u16 n <= 129, u32 n <= 2220, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Convert index to a type usable for array sizing */
    size_t N = (size_t)n;

    seq_t *dp;
#if defined(__cplusplus)
    /* In C++ value‑initialize to zero */
    dp = new seq_t[N + 1]();
#else
    /* In C use calloc to zero‑initialize */
    dp = (seq_t*)calloc(N + 1, sizeof(seq_t));
#endif
    dp[0] = 1;

    const size_t parts[] = {1, 2, 3, 5, 8};
    const size_t P = sizeof(parts) / sizeof(parts[0]);

    for (size_t p_idx = 0; p_idx < P; ++p_idx) {
        size_t p = parts[p_idx];
        for (size_t i = p; i <= N; ++i) {
            dp[i] += dp[i - p];
        }
    }

    seq_t result = dp[N];
#if defined(__cplusplus)
    delete[] dp;
#else
    free(dp);
#endif
    return result;
}
