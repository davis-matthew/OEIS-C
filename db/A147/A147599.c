/*
 * A147599  Expansion of Product_{i>=1} (1+x^(4*i-1)).
 * https://oeis.org/A147599
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 142, u16 n <= 363, u32 n <= 1038, u64 n <= 3323,
 *            u128 n <= 10000, big any n
 * Helpers:   alloc_seq_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n): number of partitions of n into distinct parts congruent to 3 (mod 4) */
/* Uses dynamic programming with a single 1‑D array of seq_t values. */

#include <stdlib.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/seq_free_array.h"

seq_t a(idx_t n)
{
    /* a(0) = 1 by convention. */
    if (n == 0)
        return (seq_t)1;

    /* Allocate an array dp[0 … n] of seq_t values. */
    seq_t *dp = alloc_seq_array(n + 1);
    if (!dp)
        return (seq_t)0;   /* allocation failure -> return 0 as safe fallback */

    /* Initialise dp. */
    dp[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        dp[i] = (seq_t)0;

    /* For each allowed part p = 3,7,11,… <= n, update dp backwards to
       ensure each part is used at most once (distinctness). */
    for (idx_t p = 3; p <= n; p += 4) {
        for (idx_t i = n; i >= p; --i) {
            dp[i] = dp[i] + dp[i - p];
        }
    }

    seq_t result = dp[n];
    seq_free_array(dp);
    return result;
}
