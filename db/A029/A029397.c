/*
 * A029397  Expansion of 1/((1-x^5)*(1-x^7)*(1-x^9)*(1-x^10)).
 * https://oeis.org/A029397
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Number of partitions of n into parts 5, 7, 9, and 10.
 *            Computation: dynamic programming (DP) with 4 loops for the parts
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *dp = NULL;   /* array of partition counts up to current maximum */
static idx_t dp_size = 0;  /* number of entries currently allocated (dp_size >= 1) */

/*  a(n): return number of partitions of n using parts 5,7,9,10. */
seq_t a(idx_t n)
{
    /* If the current table is too small, rebuild it up to n. */
    if (n >= dp_size) {
        idx_t new_size = n + 1;

        /* Allocate and zero-initialize the new table. */
        seq_t *new_dp = (seq_t *)calloc(new_size, sizeof(seq_t));
        if (!new_dp) return 0;    /* allocation failed – give up (highly unlikely) */

        /* Base case: empty partition of size 0. */
        new_dp[0] = (seq_t)1;

        /* Parts allowed in the partitions. */
        const idx_t parts[4] = {5, 7, 9, 10};

        /* DP: for each part, add contributions to all positions. */
        for (int p = 0; p < 4; ++p) {
            idx_t part = parts[p];
            for (idx_t i = part; i < new_size; ++i) {
                new_dp[i] += new_dp[i - part];
            }
        }

        /* Replace old table. */
        free(dp);
        dp     = new_dp;
        dp_size = new_size;
    }

    return dp[n];
}
