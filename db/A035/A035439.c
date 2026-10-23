/*
 * A035439  Number of partitions of n into parts 7k+4 or 7k+6.
 * https://oeis.org/A035439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Compute by dynamic programming the number of partitions of n
 *            into parts of the form 7k+4 or 7k+6 (OEIS A035439)
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *dp = NULL;      /* DP array: number of partitions for each n */
static idx_t cur_max = 0;     /* Largest n for which dp has been computed */

seq_t a(idx_t n)
{
    /* If we already have dp up to n, return the value directly. */
    if (n <= cur_max) {
        return dp[(size_t)n];
    }

    /* Otherwise recompute dp up to n.  For simplicity we recompute from scratch. */
    idx_t new_max = n;
    /* Allocate a new DP array, zero‑initialized. */
    seq_t *new_dp = (seq_t *)calloc((size_t)(new_max + 1), sizeof(seq_t));
    if (!new_dp) {
        /* Allocation failed; abort to avoid undefined behavior. */
        abort();
    }
    /* dp[0] = 1: empty partition. */
    new_dp[0] = ((seq_t)1);

    /* Iterate over all allowed part sizes up to the new maximum. */
    for (idx_t part = 4; part <= new_max; ++part) {
        /* Allowed iff part ≡ 4 or 6 (mod 7). */
        if ((part % 7ULL) == 4ULL || (part % 7ULL) == 6ULL) {
            /* Add this part to all larger totals in the DP array. */
            for (idx_t i = part; i <= new_max; ++i) {
                new_dp[(size_t)i] += new_dp[(size_t)(i - part)];
            }
        }
    }

    /* Replace the old DP array with the new one and free the old memory. */
    free(dp);
    dp = new_dp;
    cur_max = new_max;

    /* Return the computed value for the requested n. */
    return dp[(size_t)n];
}
