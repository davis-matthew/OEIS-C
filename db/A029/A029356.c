/*
 * A029356  Expansion of 1/((1-x^4)*(1-x^6)*(1-x^9)*(1-x^12)).
 * https://oeis.org/A029356
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 4, 6, 9 and 12
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *dp = NULL;          /* memoised DP table */
static idx_t dp_size = 0;         /* current allocated size (dp[0 .. dp_size-1]) */
static const idx_t PARTS[4] = {4, 6, 9, 12};

seq_t a(idx_t n)
{
    /* Allocate or extend the table to cover index n */
    if (n + 1 > dp_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(dp, new_size * sizeof(seq_t));
        if (!tmp) return 0;                /* allocation failed: return 0 */
        dp = tmp;

        /* Re‑initialise the whole table (new_size may be larger) */
        for (idx_t i = 1; i < new_size; ++i)
            dp[i] = (seq_t)0;
        dp[0] = (seq_t)1;                  /* a(0) = 1 */

        /* Classic coin‑change DP: for each part, add its contribution */
        for (int k = 0; k < 4; ++k) {
            idx_t coin = PARTS[k];
            for (idx_t i = coin; i <= n; ++i) {
                dp[i] += dp[i - coin];
            }
        }

        dp_size = new_size;
    }

    return dp[n];
}
