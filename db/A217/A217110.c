/*
 * A217110  Number of pandigital numbers with n places.
 * https://oeis.org/A217110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 9*9!*S2(n,10)
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 11, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

#define FACT (seq_t)3265920 /* 9 * 9! */

seq_t a(idx_t n)
{
    /* Cache for previously computed terms */
    static seq_t *cache = NULL;          /* array of a(i) for i ≤ cache_nmax */
    static idx_t  cache_cap = 0;         /* allocated size of cache */
    static idx_t  cache_nmax = 0;        /* maximum n for which we have values */

    /* Stirling numbers S(n, k) for k = 0..10 of the current row */
    static seq_t S[11];                  /* S[0]..S[10] */

    /* Handle n < 10: pandigital numbers do not exist */
    if (n < 10)
        return 0;

    /* Ensure cache has room for the requested n */
    if (n > cache_nmax) {
        if (cache_cap < n + 1) {
            seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
            if (!tmp) /* allocation failed; return 0 as a fallback */
                return 0;
            cache = tmp;
            cache_cap = n + 1;
        }

        /* Initialise our Stirling array if we never started before */
        if (cache_nmax == 0) {
            for (int k = 0; k <= 10; ++k)
                S[k] = 0;
            S[0] = 1;            /* S(0,0) = 1 */
        }

        /* Extend the computation row by row up to n */
        for (idx_t i = cache_nmax + 1; i <= n; ++i) {
            /* Update S(k) from k = 10 down to 1 using the recurrence
               S(n, k) = k * S(n-1, k) + S(n-1, k-1) */
            for (int k = 10; k >= 1; --k) {
                S[k] = (seq_t)k * S[k] + S[k - 1];
            }
            S[0] = 0; /* For n > 0, S(n,0) = 0 */

            /* Store the answer for this n */
            if (i >= 10)
                cache[i] = FACT * S[10];
            else
                cache[i] = 0;
        }

        cache_nmax = n;
    }

    return cache[n];
}
