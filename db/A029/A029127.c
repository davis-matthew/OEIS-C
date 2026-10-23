/*
 * A029127  Expansion of 1/((1-x)(1-x^8)(1-x^9)(1-x^10)).
 * https://oeis.org/A029127
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = coefficient of x^n in 1/((1-x)(1-x^8)(1-x^9)(1-x^10))
 *            Equivalent to: number of unordered representations of n as a sum
 *            of 1’s, 8’s, 9’s and 10’s with unlimited multiplicity. It can be
 *            computed by dynamic programming: a(0)=1; for each coin in
 *            {1,8,9,10}: for i = coin .. n: a[i] += a[i-coin];
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

static const idx_t COINS[4] = {1, 8, 9, 10};

seq_t a(idx_t n)
{
    /* Allocate an array large enough to hold all intermediate values. */
    size_t sz = (size_t)(n + 1);
    seq_t *dp = seq_alloc_new_array(sz);

    /* Initialise the table. */
    dp[0] = (seq_t)1;
    for (size_t i = 1; i < sz; ++i)
        dp[i] = (seq_t)0;

    /* Dynamic programming loops. */
    for (int k = 0; k < 4; ++k) {
        idx_t coin = COINS[k];
        for (idx_t i = coin; i <= n; ++i)
            dp[i] += dp[i - coin];
    }

    seq_t result = dp[n];
    seq_free_array(dp);
    return result;
}
