/*
 * A029444  Expansion of 1/((1-x^8)(1-x^9)(1-x^11)(1-x^12)).
 * https://oeis.org/A029444
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of non‑negative integer solutions of 8a + 9b + 11c
 *            + 12d = n. This is the coefficient of xⁿ in
 *            1/((1-x⁸)(1-x⁹)(1-x¹¹)(1-x¹²))
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   alloc_seq_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/seq_free_array.h"

seq_t a(idx_t n)
{
    /* Allocate array of dp[0..n] */
    seq_t *dp = alloc_seq_array((size_t)n + 1);
    if (!dp)
        return 0;                     /* allocation failed */

    /* initialise */
    for (idx_t i = 0; i <= n; ++i)
        dp[i] = 0;
    dp[0] = 1;

    /* the coin denominations */
    static const int coins[4] = {8, 9, 11, 12};

    /* unbounded coin change DP – combinations only */
    for (int c = 0; c < 4; ++c)
    {
        idx_t coin = (idx_t)coins[c];
        for (idx_t i = coin; i <= n; ++i)
            dp[i] += dp[i - coin];
    }

    seq_t result = dp[n];
    seq_free_array(dp);
    return result;
}
