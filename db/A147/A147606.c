/*
 * A147606  Expansion of g.f.: 1/((1 - x - x^2 + x^4 - x^6)*(1 - x^2 + x^4 +
 *          x^5 - x^6)).
 * https://oeis.org/A147606
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 27, u32 n <= 54, u64 n <= 109,
 *            u128 n <= 218, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = a(n-1) + 2*a(n-2) - a(n-3) - 3*a(n-4) + 5*a(n-6)
 *        - 3*a(n-8) - a(n-9) + 2*a(n-10) + a(n-11) - a(n-12)
 *recurrence comes from the generating function 1/((1-x-x^2+x^4-x^6)
 *                                            *(1-x^2+x^4+x^5-x^6)).

   Initial values: a(0)=1, a(1)=1, a(2)=3, a(3)=4, a(4)=6, a(5)=8,
   a(6)=12, a(7)=15, a(8)=25, a(9)=35, a(10)=56, a(11)=84.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to memoize computed terms; growing on demand */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* ensure the initial terms are present */
    if (cache_len == 0) {
        const seq_t init[12] = {1, 1, 3, 4, 6, 8, 12, 15, 25, 35, 56, 84};
        const size_t init_len = 12;
        /* allocate at least as large as the initial array */
        cache_cap = init_len;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0; /* allocation failure: undefined behaviour */
        for (size_t i = 0; i < init_len; ++i)
            cache[i] = init[i];
        cache_len = init_len;
    }

    /* expand cache if n is beyond current capacity */
    size_t required = (size_t)n + 1;
    if (required > cache_cap) {
        size_t new_cap = required;
        if (new_cap < cache_cap * 2) new_cap = cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failure: undefined behaviour */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute terms up to n if necessary */
    if (required > cache_len) {
        for (size_t i = cache_len; i < required; ++i) {
            /* recurrence requires indices i-12 .. i-1 to be valid */
            seq_t val =
                /* a(i-1) */ cache[i - 1]          +
                /* 2*a(i-2) */ 2 * cache[i - 2]     -
                /* a(i-3) */ cache[i - 3]           -
                /* 3*a(i-4) */ 3 * cache[i - 4]     +
                /* 0*a(i-5) */ 0 * cache[i - 5]     +
                /* 5*a(i-6) */ 5 * cache[i - 6]     +
                /* 0*a(i-7) */ 0 * cache[i - 7]     -
                /* 3*a(i-8) */ 3 * cache[i - 8]     -
                /* a(i-9) */ cache[i - 9]           +
                /* 2*a(i-10) */ 2 * cache[i - 10]   +
                /* a(i-11) */ cache[i - 11]         -
                /* a(i-12) */ cache[i - 12];
            cache[i] = val;
        }
        cache_len = required;
    }

    return cache[(size_t)n];
}
