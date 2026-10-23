/*
 * A029263  Expansion of 1/((1-x^3)*(1-x^4)*(1-x^8)*(1-x^10)).
 * https://oeis.org/A029263
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into the parts 3, 4, 8 and 10.
 *            Computed by a standard one‑dimensional dynamic programming that
 *            counts unordered partitions
 * Range:     u8 n <= 99, u16 n <= 709, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

static seq_t *dp = NULL;          /* table of partitions up to current size */
static idx_t  dp_size = 0;        /* number of entries stored (max index + 1) */

seq_t a(idx_t n)
{
    /* if the table is too small, (re)build it up to n */
    if (n + 1 > dp_size) {
        seq_t *new_dp = alloc_seq_array(n + 1);
        /* initialise all entries to zero */
        for (idx_t i = 0; i <= n; ++i)
            new_dp[i] = 0;
        new_dp[0] = 1;               /* one empty partition of 0 */

        /* list of available parts */
        static const idx_t parts[4] = {3, 4, 8, 10};

        /* main DP: outer loop over parts to avoid double counting */
        for (int p_idx = 0; p_idx < 4; ++p_idx) {
            idx_t part = parts[p_idx];
            for (idx_t j = part; j <= n; ++j)
                new_dp[j] += new_dp[j - part];
        }

        /* release old table and store the new one */
        if (dp)
            free_seq_array(dp);
        dp = new_dp;
        dp_size = n + 1;
    }

    return dp[n];
}
