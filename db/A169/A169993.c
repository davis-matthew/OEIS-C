/*
 * A169993  Expansion of Product_{i=0..m-1} (1 + x^(2*i+1)) for m=10.
 * https://oeis.org/A169993
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = coefficient of x^n in ∏_{i=0}^{9} (1 + x^(2*i+1))
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* number of partitions of n into distinct odd parts ≤ 19 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* DP array for sums 0..100 (max sum of 1+3+…+19) */
    static seq_t dp[101];
    static bool init = false;

    if (!init) {
        /* initialise to zero */
        for (idx_t i = 0; i < 101; ++i)
            dp[i] = 0;
        dp[0] = 1;

        /* add each odd number once (distinct parts) */
        for (idx_t i = 0; i < 10; ++i) {
            idx_t val = 2 * i + 1;               /* 1,3,…,19 */
            for (idx_t s = 100; s >= val; --s) {
                dp[s] += dp[s - val];
            }
        }
        init = true;
    }

    if (n > 100)
        return 0;     /* coefficients beyond 100 are zero */
    return dp[(idx_t)n];
}
