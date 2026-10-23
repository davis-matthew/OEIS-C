/*
 * A029102  Expansion of 1/((1-x)*(1-x^5)*(1-x^9)*(1-x^12)).
 * https://oeis.org/A029102
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of partitions of n into parts 1, 5, 9 and 12
 *   (coefficient of xⁿ in 1/((1-x)(1-x⁵)(1-x⁹)(1-x¹²))).
 * This implementation uses an incremental dynamic‑programming table.
 * For each new n requested the table is recomputed up to n from scratch.
 */

#include <stdlib.h>

static seq_t *dp = NULL;        /* memoised values (reallocated on demand) */
static idx_t  dp_size = 0;      /* current allocated size of dp  */

static const int coins[4] = {1, 5, 9, 12};

seq_t a(idx_t n)
{
    /* Allocate or reallocate table large enough for indices 0..n */
    if (dp == NULL || n + 1 > dp_size) {
        /* free previous table if it existed */
        if (dp != NULL) {
            free(dp);
        }
        /* (void*)* cast suppresses C++ implicit conversion error */
        dp      = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
        dp_size = n + 1;
    }

    /* Initialise DP table */
    dp[0] = 1;          /* empty partition */

    /* Dynamic programming: add each coin type */
    for (int ci = 0; ci < 4; ++ci) {
        int coin = coins[ci];
        for (idx_t i = (idx_t)coin; i <= n; ++i) {
            dp[i] = dp[i] + dp[i - coin];
        }
    }

    return dp[n];
}
