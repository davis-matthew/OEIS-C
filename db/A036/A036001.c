/*
 * A036001  Number of partitions of n into parts not of the form 25k, 25k+2 or
 *          25k-2. Also number of partitions with 1 part of size 1 and
 *          differences between parts at distance 11 are greater than 1.
 * https://oeis.org/A036001
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

/* Sequence A036001: partitions of n into parts not congruent to 0, ±2 (mod 25).
   Computed by a simple unrestricted partition DP:   
   dp[0] = 1; for each allowed part s ≤ n: for i = s … n  dp[i] += dp[i‑s].
   The function memoises dp up to the maximum argument received so far. */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *dp = NULL;   /* memoised DP array */
static idx_t max_n = 0;    /* largest n for which dp[] is computed */

seq_t a(idx_t n) {
    if (n == 0) return 0; /* the sequence is defined for n ≥ 1; 0 is unused */

    /* Re‑compute DP if we need a larger n than before */
    if (n > max_n) {
        idx_t new_size = n + 1;
        seq_t *new_dp = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!new_dp) exit(1);                   /* malloc failure */
        for (idx_t i = 0; i < new_size; i++) new_dp[i] = 0; /* initialise */
        new_dp[0] = 1;                           /* empty partition */

        /* Unrestricted partition DP excluding forbidden part sizes */
        for (idx_t s = 1; s <= n; s++) {
            idx_t r = s % 25;
            if (r == 0 || r == 2 || r == 23) continue; /* forbidden residue */
            for (idx_t i = s; i <= n; i++) {
                new_dp[i] += new_dp[i - s];
            }
        }

        /* Replace old table */
        if (dp) free(dp);
        dp = new_dp;
        max_n = n;
    }

    return dp[n];
}
