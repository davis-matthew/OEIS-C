/*
 * A212250  Number of (w,x,y,z) with all terms in {1,...,n} and 3w>=x+y+z+n.
 * https://oeis.org/A212250
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            4*a(n-1)-6*a(n-2)+5*a(n-3)-5*a(n-4)+6*a(n-5)-4*a(n-6)+a(n-7)
 *            with a(0)=0, a(1)=0, a(2)=4, a(3)=18, a(4)=54, a(5)=129,
 *            a(6)=262
 * Range:     u8 n <= 5, u16 n <= 24, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache   = NULL;
    static idx_t  cache_cap = 0;

    /* initial values for n = 0..6 */
    static const seq_t init[7] = {0, 0, 4, 18, 54, 129, 262};

    /* trivial cases via the stored initial values */
    if (n < 7) {
        return init[n];
    }

    /* enlarge the cache if necessary */
    if (cache == NULL || n + 1 > cache_cap) {
        idx_t new_cap = (n + 1 > cache_cap ? n + 1 : cache_cap);
        if (cache == NULL) {
            /* first allocation */
            cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
            if (!cache) abort();
        } else {
            /* reallocation */
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
        }
        cache_cap = new_cap;

        /* initialise the first 7 entries if they were not already in the cache */
        if (cache_cap >= 7) {
            for (idx_t i = 0; i < 7; ++i) cache[i] = init[i];
        }

        /* fill remaining entries up to n */
        for (idx_t i = 7; i <= n; ++i) {
            seq_t pos = cache[i-1] * (seq_t)4
                      + cache[i-3] * (seq_t)5
                      + cache[i-5] * (seq_t)6
                      + cache[i-7];
            seq_t neg = cache[i-2] * (seq_t)6
                      + cache[i-4] * (seq_t)5
                      + cache[i-6] * (seq_t)4;
            cache[i] = pos - neg;
        }
    }
    return cache[n];
}
