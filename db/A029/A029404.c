/*
 * A029404  Expansion of 1/((1-x^5)*(1-x^8)*(1-x^9)*(1-x^11)).
 * https://oeis.org/A029404
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of partitions of n into parts 5, 8, 9, and 11.
   The result is computed with the classic coin‑change DP
   (each combination is counted once). */
#include <stdlib.h>

/* Cache for computed values of the DP array. */
static seq_t *dp = NULL;   /* pointer to array of size dp_len */
static size_t dp_len = 0;  /* current size of the cache */

/* Return the number of partitions of n into parts 5, 8, 9, and 11. */
seq_t a(idx_t n)
{
    /* Ensure we have space for indices 0 .. n. */
    size_t need = (size_t)n + 1;          /* inclusive */
    if (need > dp_len) {
        /* Reallocate a larger array. */
        seq_t *new_dp = (seq_t *)realloc(dp, need * sizeof(seq_t));
        if (new_dp == NULL) {
            abort();  /* allocation failure; acceptable for this context */
        }
        dp = new_dp;
        /* Zero all entries before rebuilding. */
        for (size_t i = 0; i < need; ++i) {
            dp[i] = (seq_t)0;
        }
        dp[0] = (seq_t)1;                /* empty partition of 0 */

        /* Parts that may be used. */
        const idx_t parts[4] = {5, 8, 9, 11};
        const size_t num_parts = 4;

        /* Standard coin‑change DP: each part contributes once. */
        for (size_t p = 0; p < num_parts; ++p) {
            idx_t part = parts[p];
            for (size_t i = (size_t)part; i < need; ++i) {
                dp[i] += dp[i - (size_t)part];
            }
        }

        dp_len = need;    /* remember size */
    }

    return dp[(size_t)n];
}
