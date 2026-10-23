/*
 * A029066  Expansion of 1/((1-x)*(1-x^4)*(1-x^5)*(1-x^9)).
 * https://oeis.org/A029066
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A029066: partitions of n into parts 1, 4, 5, 9.
   Uses the standard "coin‑change" DP that counts unordered compositions.
   For each new maximum n requested the table is recomputed from scratch
   (O(4·n) operations).  This is fast enough for the intended limits. */
#include <stdlib.h>

static seq_t *dp = NULL;          /* cached values */
static idx_t dp_cap = 0;          /* current capacity (max n + 1) */
static const idx_t parts[4] = {1, 4, 5, 9};

seq_t a(idx_t n)
{
    /* Recompute the table if we haven't computed up to n yet. */
    if (dp == NULL || dp_cap <= n) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(dp, new_cap * sizeof(seq_t));
        if (!tmp)            /* allocation failed – graceful fallback */
            return (seq_t)0;
        dp = tmp;

        /* Initialize the table. */
        for (idx_t i = 0; i < new_cap; ++i)
            dp[i] = 0;
        dp[0] = 1;

        /* Coin‑change DP for parts {1,4,5,9}. */
        for (size_t k = 0; k < 4; ++k) {
            idx_t p = parts[k];
            for (idx_t i = p; i < new_cap; ++i)
                dp[i] += dp[i - p];
        }
        dp_cap = new_cap;
    }
    return dp[n];
}
