/*
 * A008666  Expansion of g.f.: 1/((1-x^2)*(1-x^3)*(1-x^5)*(1-x^6)*(1-x^9)).
 * https://oeis.org/A008666
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into parts 2, 3, 5, 6 and 9
 *            (coin‑change dynamic programming). The implementation keeps a
 *            static array that is grown whenever a larger n is requested;
 *            each expansion recomputes all values up to that n
 * Range:     u8 n <= 43, u16 n <= 212, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *dp = NULL;           /* dynamic array of counts          */
    static idx_t max_n = 0;            /* largest n that has been computed */
    static const idx_t coins[5] = {2, 3, 5, 6, 9};

    idx_t needed = n + 1;

    /* Allocate or enlarge the working array if necessary */
    if (dp == NULL || needed > max_n + 1) {
        dp = (seq_t *)realloc(dp, (size_t)needed * sizeof(seq_t));

        /* Initialise to zero and set dp[0] = 1 before the DP loops */
        for (idx_t i = 0; i < needed; ++i)
            dp[i] = 0;
        dp[0] = (seq_t)1;

        /* Standard coin‑change DP: for each coin, add ways to supply
           larger amounts that include that coin. */
        for (int ci = 0; ci < 5; ++ci) {
            idx_t c = coins[ci];
            for (idx_t amt = c; amt < needed; ++amt)
                dp[amt] += dp[amt - c];
        }

        max_n = n;
    }

    return dp[n];
}
