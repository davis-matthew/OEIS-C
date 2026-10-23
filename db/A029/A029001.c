/*
 * A029001  Expansion of 1/((1-x)(1-x^2)(1-x^3)(1-x^7)).
 * https://oeis.org/A029001
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 1, 2, 3 and 7
 * Range:     u8 n <= 33, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: dynamic programming with memoised cache */
#include <stdlib.h>

static seq_t *dp = NULL;      /* cache array for partition counts */
static idx_t  dp_size = 0;    /* number of entries in the cache */

/* Return a(n) for any n >= 0.  The function is order‑independent and
   recomputes the cache only when a larger n is requested. */
seq_t a(idx_t n)
{
    /* Rebuild the cache iff we lack enough precomputed values. */
    if (n >= dp_size) {
        idx_t new_size = n + 1;
        seq_t *new_dp = (seq_t *)realloc(dp, new_size * sizeof(seq_t));
        if (!new_dp) abort();          /* allocation failed */

        dp = new_dp;
        /* Zero the whole range that will be recomputed. */
        for (idx_t i = 0; i <= n; ++i) dp[i] = 0;
        dp[0] = 1;                     /* base case: one partition of 0 */

        /* Process each allowed part size. */
        const int parts[4] = {1, 2, 3, 7};
        for (int k = 0; k < 4; ++k) {
            idx_t p = (idx_t)parts[k];
            for (idx_t i = p; i <= n; ++i) {
                dp[i] += dp[i - p];
            }
        }
        dp_size = new_size;
    }
    return dp[n];
}
