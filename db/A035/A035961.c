/*
 * A035961  Number of partitions of n into parts not of the form 15k, 15k+7 or
 *          15k-7. Also number of partitions with at most 6 parts of size 1
 *          and differences between parts at distance 6 are greater than 1.
 * https://oeis.org/A035961
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 47, u32 n <= 146, u64 n <= 491,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1;
   a(n) = Σ_{p allowed ≤ n} a(n‑p)   for n ≥ 1
   where allowed parts p are those with p % 15 ≠ 0, 7, 8.
   This is the standard coin‑change DP counting unordered partitions. *
   The function memoizes results for successive calls by recomputing
   the table up to the maximum n requested. */

#include <stdlib.h>
#include <stddef.h>

static seq_t *dp = NULL;     /* dp[i] holds a(i) up to the current maximum */
static idx_t  dp_size = 0;   /* number of entries currently allocated */

/* Recompute dp[0..n] from scratch */
static void compute_upto(idx_t n)
{
    seq_t *new_dp = (seq_t *)realloc(dp, (n + 1) * sizeof(seq_t));
    if (!new_dp) exit(1);
    dp = new_dp;

    /* initialize the table */
    dp[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) dp[i] = (seq_t)0;

    /* populate with the DP recurrence */
    for (idx_t p = 1; p <= n; ++p) {
        idx_t r = p % 15;
        if (r == 0 || r == 7 || r == 8) continue;   /* forbidden residue */

        for (idx_t s = p; s <= n; ++s) {
            dp[s] += dp[s - p];
        }
    }
    dp_size = n + 1;
}

seq_t a(idx_t n)
{
    if (!dp || n + 1 > dp_size) {
        compute_upto(n);
    }
    return dp[n];
}
