/*
 * A029028  Expansion of 1/((1-x)(1-x^2)(1-x^9)(1-x^11)).
 * https://oeis.org/A029028
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into parts 1, 2, 9 and 11
 * Range:     u8 n <= 53, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static const idx_t _parts[4] = {1, 2, 9, 11};

seq_t a(idx_t n)
{
    // Allocate array dp[0..n] and initialize to zero.
    seq_t *dp = (seq_t*)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!dp) return 0;     // In case of allocation failure.
    dp[0] = 1;             // Base case: one way to make 0.

    // Standard unbounded coin change DP (order of coins outer, counts combinations).
    for (int pi = 0; pi < 4; ++pi) {
        idx_t p = _parts[pi];
        if (p > n) continue;
        for (idx_t i = p; i <= n; ++i) {
            dp[i] = dp[i] + dp[i - p];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
