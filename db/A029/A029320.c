/*
 * A029320  Expansion of 1/((1-x^3)*(1-x^8)*(1-x^10)*(1-x^12)).
 * https://oeis.org/A029320
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 3, 8, 10, and 12.
 *            Computed by dynamic programming: for each part p, for i = p..n
 *            dp[i] += dp[i - p]. Complexity O(n)
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *dp = NULL;      /* cached DP table */
static idx_t cur_max = 0;     /* maximum n for which dp is valid */

seq_t a(idx_t n)
{
    /* Rebuild the table if it does not exist or the requested n is larger. */
    if (dp == NULL || n > cur_max) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(dp, new_size * sizeof(seq_t));
        if (!tmp) {                 /* allocation failed */
            free(dp);
            dp = NULL;
            cur_max = 0;
            return 0;
        }
        dp = tmp;

        /* Initialize the whole table up to n. */
        for (idx_t i = 0; i <= n; ++i)
            dp[i] = 0;
        dp[0] = 1;

        /* Dynamic programming for the four parts. */
        static const idx_t parts[4] = {3, 8, 10, 12};
        for (idx_t p_ix = 0; p_ix < 4; ++p_ix) {
            idx_t p = parts[p_ix];
            for (idx_t i = p; i <= n; ++i) {
                dp[i] += dp[i - p];
            }
        }
        cur_max = n;
    }
    /* For n <= cur_max the value is already computed. */
    return dp[n];
}
