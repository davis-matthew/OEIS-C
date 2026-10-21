/*
 * A060024  Expansion of (1-x-x^N)/((1-x)(1-x^2)(1-x^3)...(1-x^N)) for N = 5.
 * https://oeis.org/A060024
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-1) + a(n-2) - a(n-5) - a(n-6) - a(n-7) + a(n-8) +
 *            a(n-9) + a(n-10) - a(n-13) - a(n-14) + a(n-15) with a(0)=1,
 *            a(1)=0, a(2)=1, a(3)=1, a(4)=2, a(5)=1, a(6)=2, a(7)=1, a(8)=2,
 *            a(9)=0, a(10)=0, a(11)=-3, a(12)=-3, a(13)=-8, a(14)=-10,
 *            a(15)=-16
 * Range:     i8 n <= 24, i16 n <= 97, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute a(n) for OEIS A060024, N = 5.  The function is
   memoised and works for any n >= 0, in any call order. */

seq_t a(idx_t n)
{
    /* lazy initialisation and caching */
    static seq_t *cache = NULL;   /* stores values a(0) .. a(cache_len-1) */
    static idx_t cache_len = 0;   /* number of stored values   */
    static idx_t cache_cap = 0;   /* allocated capacity        */

    /* initialise the first 16 known terms */
    if (cache_cap == 0) {
        cache_cap = 16;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) {            /* allocation failed – return a default */
            return (seq_t)0;
        }
        cache[0] =  1; cache[1] =  0; cache[2] =  1;
        cache[3] =  1; cache[4] =  2; cache[5] =  1;
        cache[6] =  2; cache[7] =  1; cache[8] =  2;
        cache[9] =  0; cache[10] = 0; cache[11] = -3;
        cache[12] = -3; cache[13] = -8; cache[14] = -10;
        cache[15] = -16;
        cache_len = cache_cap;
    }

    /* grow the cache if required and compute missing terms */
    if (n >= cache_len) {
        idx_t needed = n + 1;
        if (needed > cache_cap) {
            /* double the capacity until it fits */
            idx_t new_cap = cache_cap;
            while (new_cap < needed) {
                new_cap *= 2;
            }
            seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!new_cache) {
                /* memory failure – return a default value */
                return (seq_t)0;
            }
            cache = new_cache;
            cache_cap = new_cap;
        }
        /* compute all terms up to n */
        for (idx_t k = cache_len; k <= n; ++k) {
            seq_t val = cache[k-1] + cache[k-2]
                      - cache[k-5] - cache[k-6] - cache[k-7]
                      + cache[k-8] + cache[k-9] + cache[k-10]
                      - cache[k-13] - cache[k-14]
                      + cache[k-15];
            cache[k] = val;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
