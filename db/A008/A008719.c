/*
 * A008719  Expansion of 1/((1-x)*(1-x^4)*(1-x^6)*(1-x^12)).
 * https://oeis.org/A008719
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   number of partitions of n using parts 1, 4, 6, and 12. a(n) =
 *            coefficient of x^n in 1/((1-x)*(1-x^4)*(1-x^6)*(1-x^12))
 * Range:     u8 n <= 65, u16 n <= 471, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *dp = NULL;
static idx_t dp_size = 0;

seq_t a(idx_t n)
{
    if (n >= dp_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(dp, new_size * sizeof(seq_t));
        if (!tmp) exit(1);
        dp = tmp;
        dp[0] = (seq_t)1;
        for (idx_t i = 1; i < new_size; ++i)
            dp[i] = (seq_t)0;
        const idx_t coins[4] = {1, 4, 6, 12};
        for (int k = 0; k < 4; ++k) {
            idx_t coin = coins[k];
            for (idx_t i = coin; i < new_size; ++i)
                dp[i] += dp[i - coin];
        }
        dp_size = new_size;
    }
    return dp[n];
}
