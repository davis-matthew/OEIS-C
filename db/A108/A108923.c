/*
 * A108923  Expansion of 1/((x^8+1)*(1-x)^3).
 * https://oeis.org/A108923
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + a(n-3) - a(n-8) + 3*a(n-9) -
 *            3*a(n-10) + a(n-11); Initial terms: a(0)=1, a(1)=3, a(2)=6,
 *            a(3)=10, a(4)=15, a(5)=21, a(6)=28, a(7)=36, a(8)=44, a(9)=52,
 *            a(10)=60
 * Range:     u8 n <= 26, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init[12] = {1, 3, 6, 10, 15, 21, 28, 36, 44, 52, 60, 68};

seq_t a(idx_t n)
{
    /* Handle initial values directly */
    if (n < 12)
        return init[n];

    /* Static cache of computed terms */
    static seq_t *dp = NULL;
    static idx_t max_computed = 0;

    /* First time: allocate and compute up to n */
    if (dp == NULL) {
        dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        if (dp == NULL)
            abort(); /* allocation failed */

        for (idx_t i = 0; i < 12; ++i)
            dp[i] = init[i];

        for (idx_t i = 12; i <= n; ++i) {
            seq_t t = dp[i - 1] * 3;
            t -= dp[i - 2] * 3;
            t += dp[i - 3];
            t -= dp[i - 8];
            t += dp[i - 9] * 3;
            t -= dp[i - 10] * 3;
            t += dp[i - 11];
            dp[i] = t;
        }

        max_computed = n;
        return dp[n];
    }

    /* Cached array exists */
    if (n <= max_computed)
        return dp[n];

    /* Grow cache to size n+1 */
    dp = (seq_t *)realloc(dp, (n + 1) * sizeof(seq_t));
    if (dp == NULL)
        abort();

    /* Compute missing terms */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t t = dp[i - 1] * 3;
        t -= dp[i - 2] * 3;
        t += dp[i - 3];
        t -= dp[i - 8];
        t += dp[i - 9] * 3;
        t -= dp[i - 10] * 3;
        t += dp[i - 11];
        dp[i] = t;
    }

    max_computed = n;
    return dp[n];
}
