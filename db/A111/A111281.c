/*
 * A111281  Number of permutations avoiding the patterns
 *          {2413,2431,4213,3412,3421,4231,4321,4312}; number of strong
 *          sorting class based on 2413.
 * https://oeis.org/A111281
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) + 2*a(n-3) Initial terms: a(0)=1,
 *            a(1)=1, a(2)=2
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 25, u64 n <= 48, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t *dp = NULL;       /* memoization array */
    static idx_t cache_len = 0;   /* how many terms are filled */

    /* If already computed, return it */
    if (n < cache_len)
        return dp[n];

    /* Allocate or extend storage to cover n */
    idx_t needed = n + 1;
    dp = (seq_t *)realloc(dp, sizeof(seq_t) * needed);
    if (!dp)
        abort();                  /* cannot allocate more memory */

    /* Compute missing terms */
    for (idx_t i = cache_len; i <= n; ++i)
    {
        if (i == 0)
            dp[i] = (seq_t)1;
        else if (i == 1)
            dp[i] = (seq_t)1;
        else if (i == 2)
            dp[i] = (seq_t)2;
        else
            dp[i] = 3 * dp[i - 1] - 2 * dp[i - 2] + 2 * dp[i - 3];
    }

    cache_len = needed;
    return dp[n];
}
