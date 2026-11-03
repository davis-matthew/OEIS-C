/*
 * A116492  Number of partitions of n into distinct odious numbers.
 * https://oeis.org/A116492
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into distinct odious numbers *
 *            (odious numbers = integers with an odd number of 1‑bits)
 * Range:     u8 n <= 60, u16 n <= 160, u32 n <= 481, u64 n <= 1594,
 *            u128 n <= 5663, big any n
 * Validated: standard, pass, 85 terms through n = 9919 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include <string.h>
#include <stddef.h>
#include <math.h>
#include "../utils/mem.h"

/* C allocation helpers */
static seq_t* seq_alloc_new_array(size_t n)   { return mem_new(seq_t, n); }

/* Return the n-th term of OEIS sequence A116492.  */
seq_t a(idx_t n)
{
    /* Allocate DP array: dp[s] = number of ways to form sum s. */
    seq_t *dp = seq_alloc_new_array((size_t)n + 1);
    for (idx_t i = 0; i <= n; ++i) dp[i] = 0;
    dp[0] = 1;

    /* Process each odious number k (1 ≤ k ≤ n).  */
    for (idx_t k = 1; k <= n; ++k) {
        /* Check if k is odious: odd popcount of its bit pattern. */
        if (__builtin_popcountll((unsigned long long)k) & 1) {
            for (idx_t s = n; s >= k; --s) {
                dp[s] += dp[s - k];
            }
        }
    }

    seq_t result = dp[n];
    mem_free(dp);
    return result;
}
