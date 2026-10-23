/*
 * A029437  Expansion of 1/((1-x^7)(1-x^8)(1-x^11)(1-x^12)).
 * https://oeis.org/A029437
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 205, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Coefficient of 1/((1-x^7)(1-x^8)(1-x^11)(1-x^12)) in the power series.
   a(n) equals the number of non‑negative integer solutions of
   7c1 + 8c2 + 11c3 + 12c4 = n. */
#include <stdlib.h>

static seq_t *dp = NULL;   /* dynamic array of a(0)…a(k) */
static idx_t  dp_size = 0; /* current number of stored entries */

/* Expand dp to hold all values up to `limit` inclusive, recomputing
   the table from scratch for correctness. */
static void rebuild_to(idx_t limit)
{
    const idx_t coins[4] = {7, 8, 11, 12};
    idx_t new_cap = limit + 1;

    seq_t *new_dp = (seq_t *)realloc(dp, new_cap * sizeof(seq_t));
    if (!new_dp) abort();
    dp = new_dp;

    /* initialise all entries to 0 and set the base case */
    for (idx_t i = 0; i < new_cap; ++i)
        dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;

    /* standard coin‑change DP */
    for (int c = 0; c < 4; ++c) {
        const idx_t d = coins[c];
        for (idx_t i = d; i <= limit; ++i) {
            dp[i] += dp[i - d];
        }
    }
    dp_size = new_cap;
}

seq_t a(idx_t n)
{
    if (n >= dp_size) rebuild_to(n);
    return dp[n];
}
