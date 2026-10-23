/*
 * A029202  Expansion of 1/((1-x^2)*(1-x^5)*(1-x^7)*(1-x^11)).
 * https://oeis.org/A029202
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts {2,5,7,11}
 * Range:     u8 n <= 93, u16 n <= 659, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (OEIS A029202, offset 0) */
#include <stdlib.h>

static seq_t *dp = NULL;          /* DP array holding partition counts */
static idx_t  dp_size = 0;        /* length of the allocated array */

static const idx_t parts[4] = {2, 5, 7, 11};

seq_t a(idx_t n)
{
    /* Recompute the DP array if necessary */
    if (dp == NULL || n + 1 > dp_size) {
        idx_t new_size = n + 1;
        seq_t *new_dp = (seq_t *)malloc((size_t)new_size * sizeof(seq_t));
        if (!new_dp) return 0;                     /* out‑of‑memory safeguard */

        /* Initialise all entries to zero */
        for (idx_t i = 0; i < new_size; ++i)
            new_dp[i] = 0;

        new_dp[0] = 1;   /* one way to partition 0 */

        /* Classic “coin change” DP: outer loop over parts, inner over sums */
        for (int k = 0; k < 4; ++k) {
            idx_t part = parts[k];
            for (idx_t i = part; i < new_size; ++i) {
                new_dp[i] += new_dp[i - part];
            }
        }

        /* Replace the old table and free memory */
        if (dp) free(dp);
        dp = new_dp;
        dp_size = new_size;
    }

    /* Lookup the precomputed result */
    return dp[n];
}
