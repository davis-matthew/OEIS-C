/*
 * A222099  Number of n X 3 0..5 arrays with entries increasing mod 6 by 0, 1,
 *          2 or 3 rightwards and downwards, starting with upper left zero.
 * https://oeis.org/A222099
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 42*a(n-1) - 302*a(n-2) + 521*a(n-3) + 114*a(n-4) -
 *            144*a(n-5) for n ≥ 6. Initial values: a(1)=16 , a(2)=530 ,
 *            a(3)=17694 , a(4)=591476 , a(5)=19776286
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset: a(1) is first term */

    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cap  = 0;        /* allocated capacity */
    static size_t len  = 0;        /* number of terms stored */

    /* expand cache if necessary */
    if ((idx_t)len < n) {
        if (cap < (size_t)n) {
            size_t newcap = cap ? cap << 1 : 8;
            if (newcap < (size_t)n) newcap = (size_t)n;
            seq_t *tmp = (seq_t*)malloc(newcap * sizeof(seq_t));
            if (cache) {
                memcpy(tmp, cache, len * sizeof(seq_t));
                free(cache);
            }
            cache = tmp;
            cap   = newcap;
        }

        /* initialise first five terms once */
        if (len == 0) {
            cache[0] = (seq_t)16;
            cache[1] = (seq_t)530;
            cache[2] = (seq_t)17694;
            cache[3] = (seq_t)591476;
            cache[4] = (seq_t)19776286;
            len     = 5;
        }

        /* compute missing terms up to n */
        for (idx_t i = len + 1; i <= n; ++i) {
            if (i <= 5) /* already initialised */ continue;

            /* recurrence */
            seq_t term =
                (seq_t)42  * cache[i-2]   /* a(n-1) */
              - (seq_t)302 * cache[i-3]   /* a(n-2) */
              + (seq_t)521 * cache[i-4]   /* a(n-3) */
              + (seq_t)114 * cache[i-5]   /* a(n-4) */
              - (seq_t)144 * cache[i-6];  /* a(n-5) */

            cache[i-1] = term;           /* store a(n) */
        }
        if ((idx_t)len < n) len = n;
    }

    return cache[n-1];
}
