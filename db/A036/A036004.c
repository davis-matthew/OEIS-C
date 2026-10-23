/*
 * A036004  Number of partitions of n into parts not of the form 25k, 25k+5 or
 *          25k-5. Also number of partitions with at most 4 parts of size 1
 *          and differences between parts at distance 11 are greater than 1.
 * https://oeis.org/A036004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A036004 by dynamic programming.
 * a(n) = number of partitions of n into parts not congruent to
 *        0, 5, or 20 modulo 25 (i.e. 25k, 25k+5, 25k-5). */
#include <stdint.h>
#include <stdlib.h>

/* Static table holding partition counts up to the highest n
 * computed during this process. */
static idx_t current_max_n = 0;
static seq_t *dp = NULL;

/* Returns the number of partitions of n under the given restriction. */
seq_t a(idx_t n)
{
    /* Offset is 1; for n==0 the sequence is not defined. */
    if (n == (idx_t)0)
        return (seq_t)0;

    /* If we already have values up to n, answer immediately. */
    if (n <= current_max_n)
        return dp[n];

    /* Allocate a new table large enough for the new maximum. */
    seq_t *new_dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (new_dp == NULL) {
        /* Out of memory – abort in a predictable way. */
        abort();
    }

    /* Initialise array to zero, and set dp[0] = 1 (empty partition). */
    for (idx_t i = 0; i <= n; ++i)
        new_dp[i] = 0;
    new_dp[0] = 1;

    /* Classic partition DP: for each allowed part p, update all i >= p. */
    for (idx_t p = 1; p <= n; ++p) {
        idx_t r = p % 25;
        if (r == 0 || r == 5 || r == 20)
            continue;               /* skip forbidden part types */
        for (idx_t i = p; i <= n; ++i) {
            new_dp[i] += new_dp[i - p];
        }
    }

    /* Replace the old table by the new one. */
    free(dp);
    dp = new_dp;
    current_max_n = n;

    return dp[n];
}
