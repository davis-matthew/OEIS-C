/*
 * A029431  Expansion of 1/((1-x^6)(1-x^10)(1-x^11)(1-x^12)).
 * https://oeis.org/A029431
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 6,10,11,12
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *dp = NULL;
    static idx_t dp_size = 0;   /* number of elements currently allocated */

    /* Recompute the entire table each time we need a larger n. */
    if (n >= dp_size) {
        idx_t new_size = n + 1;
        seq_t *new_dp = (seq_t *)realloc(dp, new_size * sizeof(seq_t));
        if (!new_dp)               /* allocation failed – return 0 */
            return (seq_t)0;
        dp = new_dp;

        /* initialise */
        for (idx_t i = 0; i < new_size; ++i)
            dp[i] = (seq_t)0;
        dp[0] = (seq_t)1;

        /* parts that generate powers of x in the product */
        static const idx_t parts[4] = { 6, 10, 11, 12 };

        /* classic coin‑change DP */
        for (int p = 0; p < 4; ++p) {
            idx_t coin = parts[p];
            for (idx_t i = coin; i < new_size; ++i)
                dp[i] += dp[i - coin];
        }

        dp_size = new_size;
    }

    return dp[n];
}
