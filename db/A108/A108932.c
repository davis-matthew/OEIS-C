/*
 * A108932  Number of partitions of n into parts that are congruent to 1, 5 or
 *          6 mod 8.
 * https://oeis.org/A108932
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into parts congruent to * 1, 5,
 *            or 6 modulo 8 (OEIS A108932). Computed by a simple *
 *            dynamic‑programming sieve
 * Range:     u8 n <= 39, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   seq_malloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_malloc.h"

seq_t a(idx_t n)
{
    /* Allocate DP array of length n+1. */
    seq_t *dp = seq_malloc(n + 1);
    if (!dp) return 0;                 /* allocation failed */

    /* Initialise: only dp[0] = 1, the rest 0. */
    dp[0] = 1;
    for (idx_t i = 1; i <= n; ++i) dp[i] = 0;

    /* Process part sizes ≡ 1 (mod 8). */
    for (idx_t p = 1; p <= n; p += 8) {
        for (idx_t i = p; i <= n; ++i) {
            dp[i] += dp[i - p];
        }
    }

    /* Process part sizes ≡ 5 (mod 8). */
    for (idx_t p = 5; p <= n; p += 8) {
        for (idx_t i = p; i <= n; ++i) {
            dp[i] += dp[i - p];
        }
    }

    /* Process part sizes ≡ 6 (mod 8). */
    for (idx_t p = 6; p <= n; p += 8) {
        for (idx_t i = p; i <= n; ++i) {
            dp[i] += dp[i - p];
        }
    }

    seq_t result = dp[n];
    free(dp);                          /* release temporary array */
    return result;
}
