/*
 * A035980  Number of partitions of n into parts not of the form 21k, 21k+2 or
 *          21k-2. Also number of partitions with 1 part of size 1 and
 *          differences between parts at distance 9 are greater than 1.
 * https://oeis.org/A035980
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A035980: Number of partitions of n into parts not congruent to
 * 0, 2, or 19 modulo 21.
 * 
 * Computed using the standard coin‑change DP algorithm:
 *   dp[0] = 1
 *   For each allowed part size k (k % 21 ∉ {0,2,19}) in increasing order:
 *     For i = k .. n: dp[i] += dp[i - k]
 *   The table is recomputed from scratch every time the requested
 *   n increases.
 */
#include <stdlib.h>

static seq_t *dp = NULL;            /* array of a(0)..a(max_computed) */
static idx_t max_computed = 0;      /* largest n for which dp is valid */

/* Recompute dp[0..n] when n exceeds the current maximum. */
static void ensure_dp(idx_t n)
{
    if (n <= max_computed) return;          /* already up to date */

    /* allocate a new table of required size */
    size_t size = (size_t)(n + 1);
    seq_t *new_dp = (seq_t *)malloc(size * sizeof(seq_t));
    if (!new_dp) exit(EXIT_FAILURE);

    /* initialize */
    new_dp[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) new_dp[i] = 0;

    /* dynamic programming: process each allowed part size */
    for (idx_t part = 1; part <= n; ++part) {
        idx_t r = part % 21;
        if (r == 0 || r == 2 || r == 19) continue;   /* forbidden residue */

        for (idx_t i = part; i <= n; ++i) {
            new_dp[i] += new_dp[i - part];
        }
    }

    free(dp);
    dp = new_dp;
    max_computed = n;
}

/* External function required by the harness. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* a(0) is not defined in the sequence */
    ensure_dp(n);
    return dp[n];
}
