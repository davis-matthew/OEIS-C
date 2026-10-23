/*
 * A008643  Molien series for group of 4 X 4 upper triangular matrices over
 *          GF(2).
 * https://oeis.org/A008643
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 1, 2, 4, and 8 (OEIS
 *            A008643)
 * Range:     u8 n <= 39, u16 n <= 285, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *dp = NULL;      /* dynamic array used for computation */
static idx_t dp_cap = 0;      /* current capacity of dp */

/* Compute a(n) by a simple dynamic‑programming coin‑change algorithm. */
seq_t a(idx_t n)
{
    /* Ensure the table is large enough. */
    if (n + 1 > dp_cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(dp, new_cap * sizeof(seq_t));
        if (!tmp) /* allocation failure: leave old table intact */
            return 0;
        dp = tmp;
        dp_cap = (idx_t)new_cap;
    }

    /* Initialize the table. */
    dp[0] = 1;
    for (idx_t i = 1; i <= n; ++i)
        dp[i] = 0;

    /* Parts allowed. The order of loops is fixed to avoid double counting. */
    static const idx_t parts[4] = {1, 2, 4, 8};

    for (int pidx = 0; pidx < 4; ++pidx) {
        idx_t p = parts[pidx];
        for (idx_t i = p; i <= n; ++i) {
            dp[i] += dp[i - p];
        }
    }

    return dp[n];
}
