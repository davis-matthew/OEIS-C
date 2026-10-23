/*
 * A029104  Expansion of 1/((1-x)*(1-x^5)*(1-x^10)*(1-x^12)).
 * https://oeis.org/A029104
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Computes the number of partitions of n into parts 1, 5, 10 and 12
   (OEIS A029104).  Dynamic programming:  dp[0] = 1 and for each coin
   value c in {1,5,10,12} we add dp[i‑c] to every dp[i] with i ≥ c.
*/
#include <stdlib.h>   /* malloc, free */

seq_t a(idx_t n)
{
    /* Allocate and initialise the DP array.  calloc zeroes the array. */
    seq_t *dp = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!dp) return 0;          /* allocation failure → return 0 */
    dp[0] = 1;                   /* base case */

    /* The four allowed part sizes. */
    const idx_t coins[4] = { 1, 5, 10, 12 };

    /* Standard unbounded coin‑change DP. */
    for (size_t ci = 0; ci < 4; ++ci) {
        idx_t coin = coins[ci];
        for (idx_t i = coin; i <= n; ++i) {
            dp[i] += dp[i - coin];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
