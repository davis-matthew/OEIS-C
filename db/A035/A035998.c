/*
 * A035998  Number of partitions of n into parts not of the form 23k, 23k+10
 *          or 23k-10. Also number of partitions with at most 9 parts of size
 *          1 and differences between parts at distance 10 are greater than 1.
 * https://oeis.org/A035998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 44, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   part_allowed
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A035998: Number of partitions of n into parts not of the form
   23k, 23k+10 or 23k-10.
   The value is computed by a standard unbounded‑knapsack dynamic
   programming:  dp[i] = number of allowed partitions of i.
   Complexity:  O(n²) which is fine for n up to ∼10 000. */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/part_allowed.h"

seq_t a(idx_t n)
{
    static seq_t *dp = NULL;
    static idx_t dp_cap = 0;

    if (n == 0) return 0;          /* undefined, but harmless */

    /* Grow or rebuild the DP table when a larger n is requested. */
    if (n + 1 > dp_cap) {
        free(dp);
        dp = (seq_t*)malloc((size_t)(n + 1) * sizeof(seq_t));
        if (!dp) return 0;        /* allocation failure */
        dp_cap = n + 1;
    }

    /* Recompute the table from scratch for the current n. */
    memset(dp, 0, (size_t)(dp_cap) * sizeof(seq_t));
    dp[0] = 1;

    for (idx_t p = 1; p <= n; ++p) {
        if (!part_allowed(p))
            continue;
        for (idx_t i = p; i <= n; ++i) {
            dp[i] += dp[i - p];
        }
    }

    return dp[n];
}
