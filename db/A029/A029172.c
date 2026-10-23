/*
 * A029172  Expansion of 1/((1-x^2)*(1-x^4)*(1-x^5)*(1-x^7)).
 * https://oeis.org/A029172
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A029172: number of partitions of n into parts 2, 4, 5, and 7.
   This implementation uses a classic dynamic‑programming approach:
   dp[0] = 1; for each part p do for i = p..n dp[i] += dp[i-p].
   The table is cached statically and expanded on demand. */

#include <stdlib.h>

static seq_t *dp = NULL;      /* dp[0..max_n] holds the values a(0)...a(max_n) */
static idx_t max_n = 0;       /* largest n that has been computed so far */

/* Compute a(n) for arbitrary n, caching results for future calls. */
seq_t a(idx_t n)
{
    /* Ensure the DP table is large enough. */
    if (dp == NULL || n > max_n) {
        idx_t new_size = (max_n > n) ? max_n : n;   /* keep the larger size */
        seq_t *new_dp = (seq_t *)realloc(dp, (size_t)(new_size + 1) * sizeof(seq_t));
        if (new_dp == NULL) return (seq_t)0;             /* allocation failure */
        dp = new_dp;
        max_n = new_size;
    }

    /* Re‑compute dp[0..max_n] from scratch.  This is O(parts * max_n)
       and is fine for max_n <= 10000. */
    dp[0] = (seq_t)1;
    for (idx_t i = 1; i <= max_n; ++i) dp[i] = (seq_t)0;

    static const idx_t parts[4] = {2, 4, 5, 7};
    for (int pi = 0; pi < 4; ++pi) {
        idx_t p = parts[pi];
        for (idx_t i = p; i <= max_n; ++i) {
            dp[i] += dp[i - p];
        }
    }

    return dp[(idx_t)n];
}
