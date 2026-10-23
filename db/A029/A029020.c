/*
 * A029020  Expansion of 1/((1-x)*(1-x^2)*(1-x^7)*(1-x^8)).
 * https://oeis.org/A029020
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: coin‑change DP for partitions with parts 1, 2, 7, 8.
   The generating function is 1/((1-x)(1-x^2)(1-x^7)(1-x^8)).
   For any n, we compute dp[i] = number of partitions of i
   using the allowed parts by iterating over parts and building up dp.
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static buffer reused across calls */
    static seq_t *dp = NULL;
    static idx_t dp_size = 0;

    /* (re)allocate dp array to accommodate n */
    if (dp == NULL || dp_size <= n) {
        dp = (seq_t *)realloc(dp, (n + 1) * sizeof(seq_t));
        dp_size = n + 1;
    }

    /* zero the array and set base case */
    for (idx_t i = 0; i < dp_size; ++i)
        dp[i] = 0;
    dp[0] = 1;

    /* parts that are allowed in the partition */
    const idx_t parts[4] = {1, 2, 7, 8};

    /* standard coin–change DP (order of parts matters, but we sum
       over all parts in increasing order to count unordered partitions) */
    for (int p = 0; p < 4; ++p) {
        idx_t part = parts[p];
        for (idx_t i = part; i <= n; ++i) {
            dp[i] += dp[i - part];
        }
    }

    return dp[n];
}
