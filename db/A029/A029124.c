/*
 * A029124  Expansion of 1/((1-x)(1-x^7)(1-x^10)(1-x^11)).
 * https://oeis.org/A029124
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the number of partitions of n using parts 1,7,10,11.
 *            This equals the coefficient of x^n in
 *            1/((1-x)(1-x^7)(1-x^10)(1-x^11))
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses dynamic programming with memoisation.
   dp[i] stores the number of ways to form i using the allowed parts.
   The table is grown to the needed size on each call.
*/

#include <stdlib.h>

static seq_t *dp = NULL;     /* cache of DP values */
static idx_t cached_size = 0;/* current cache size */

/* denominations used in the generating function */
static const idx_t coins[4] = {1, 7, 10, 11};

seq_t a(idx_t n)
{
    /* Grow cache if necessary */
    if (n + 1 > cached_size) {
        /* Allocate / resize the cache */
        dp = (seq_t *)realloc(dp, (size_t)(n + 1) * sizeof(seq_t));
        if (dp == NULL) {
            /* Allocation failed – return 0 as a safe fallback */
            return (seq_t)0;
        }

        /* Initialise dp array to zero */
        for (idx_t i = 0; i <= n; ++i) {
            dp[i] = (seq_t)0;
        }

        /* Base case: one way to form 0 (use no parts) */
        dp[0] = (seq_t)1;

        /* Dynamic programming: for each coin, add its contribution */
        for (idx_t cidx = 0; cidx < 4; ++cidx) {
            idx_t coin = coins[cidx];
            for (idx_t i = coin; i <= n; ++i) {
                dp[i] += dp[i - coin];
            }
        }

        cached_size = n + 1;
    }

    return dp[n];
}
