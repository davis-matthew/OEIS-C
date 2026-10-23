/*
 * A126378  Number of base 24 n-digit numbers with adjacent digits differing
 *          by one or less.
 * https://oeis.org/A126378
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of base‑24 n‑digit strings (digits 0–23) where
 *            adjacent digits differ by at most 1. Recurrence: dp[0][d] = 1
 *            (for all d ∈ {0,…,23}); dp[t+1][d] = dp[t][d] (+ dp[t][d-1] if
 *            d>0) (+ dp[t][d+1] if d<23); a(0) = 1; a(n) = Σ_{d=0}^{23}
 *            dp[n-1][d] for n≥1
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 38, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses dynamic programming over 24 states, O(n) time and O(1) memory. */
#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* Special case: zero‑length string has one possibility. */
    if (n == 0) return (seq_t)1;

    /* Alphabet size for base 24. */
    const idx_t B = 24;

    /* DP vectors for the current and next step. */
    seq_t cur[B];
    seq_t nxt[B];

    /* Initialise for strings of length 1: each digit can start a string. */
    for (idx_t i = 0; i < B; ++i)
        cur[i] = (seq_t)1;

    /* Iterate for remaining positions (n-1 steps). */
    for (idx_t step = 1; step < n; ++step) {
        /* Zero the next vector. */
        for (idx_t i = 0; i < B; ++i)
            nxt[i] = (seq_t)0;

        /* Update counts for each possible ending digit. */
        for (idx_t d = 0; d < B; ++d) {
            /* Stay in the same digit. */
            nxt[d] += cur[d];

            /* Move from the left neighbor if it exists. */
            if (d > 0)
                nxt[d] += cur[d - 1];

            /* Move from the right neighbor if it exists. */
            if (d + 1 < B)
                nxt[d] += cur[d + 1];
        }

        /* Swap cur and nxt for the next iteration. */
        for (idx_t i = 0; i < B; ++i)
            cur[i] = nxt[i];
    }

    /* Sum all counts to get total number of valid strings of length n. */
    seq_t total = (seq_t)0;
    for (idx_t i = 0; i < B; ++i)
        total += cur[i];

    return total;
}
