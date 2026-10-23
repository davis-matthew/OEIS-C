/*
 * A029402  Expansion of 1/((1-x^5)*(1-x^7)*(1-x^11)*(1-x^12)).
 * https://oeis.org/A029402
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A029402: number of partitions of n into parts 5, 7, 11, and 12.
   Computed by the classic coin‑change DP: dp[j] = sum dp[j - part] for
   all allowed parts, with dp[0] = 1.  The DP table is cached and
   recomputed only when a larger index is requested.  All arithmetic
   uses the seq_t type supplied by the build. */
#include <stdlib.h>

static const idx_t PARTS[4] = {5, 7, 11, 12};

seq_t a(idx_t n)
{
    /* Persistent DP table and current maximum index computed. */
    static seq_t *dp = NULL;
    static idx_t maxn = 0;

    /* Allocate or grow the DP array to size n+1. */
    if (dp == NULL || n > maxn) {
        /* size_t must be used for malloc/realloc size arguments. */
        size_t newlen = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(dp, newlen * sizeof(seq_t));
        if (tmp == NULL) {
            /* Allocation failed – return zero (the sequence is non‑negative). */
            return (seq_t)0;
        }
        dp = tmp;
        maxn = n;
    }

    /* Re‑initialise the table and compute values up to n. */
    dp[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        dp[i] = (seq_t)0;
    }

    for (size_t k = 0; k < 4; ++k) {
        idx_t part = PARTS[k];
        if (part > n) continue;
        for (idx_t j = part; j <= n; ++j) {
            dp[j] += dp[j - part];
        }
    }

    return dp[n];
}
