/*
 * A106570  a(n) = 4*a(n-1) + 3*a(n-2), with a(0)=0, a(1)=3.
 * https://oeis.org/A106570
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 4*a(n-1) + 3*a(n-2), a(0)=0, a(1)=3
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static memoization table */
    static seq_t *dp = NULL;
    static idx_t cap = 0;   /* allocated capacity */
    static idx_t sz = 0;    /* number of initialized terms */

    /* initialise table at first call */
    if (dp == NULL) {
        cap = 4;                     /* reasonable starting size */
        dp = (seq_t *)calloc(cap, sizeof(seq_t));
        dp[0] = (seq_t)0;
        dp[1] = (seq_t)3;
        sz   = 2;                    /* terms 0 and 1 are ready */
    }

    /* grow and compute if we need a larger n */
    if (n >= sz) {
        /* enlarge capacity until it can hold a[n] */
        while (cap <= n) cap <<= 1;

        dp = (seq_t *)realloc(dp, cap * sizeof(seq_t));

        /* compute missing terms using the recurrence */
        for (idx_t i = sz; i <= n; ++i) {
            dp[i] = 4 * dp[i - 1] + 3 * dp[i - 2];
        }

        sz = n + 1; /* now we have terms up to a[n] */
    }

    return dp[n];
}
