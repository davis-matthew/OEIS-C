/*
 * A187065  Let i be in {1,2,3} and let r >= 0 be an integer. Let p = {p_1,
 *          p_2, p_3} = {-2,0,1}, n=2*r+p_i, and define a(-2)=1. Then,
 *          a(n)=a(2*r+p_i) gives the quantity of H_(7,1,0) tiles in a
 *          subdivided H_(7,i,r) tile after linear scaling by the factor x^r,
 *          where x=sqrt(2*cos(Pi/7)).
 * https://oeis.org/A187065
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + 2*a(n-4) - a(n-6)
 * Range:     u8 n <= 24, u16 n <= 43, u32 n <= 81, u64 n <= 156,
 *            u128 n <= 307, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values: a(0)=0, a(1)=0, a(2)=1, a(3)=0,
 *                 a(4)=0, a(5)=1, a(6)=2             */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of elements already stored */

    /* helper to grow cache when needed */
    if (cache == NULL) {
        /* initial capacity */
        size_t init_cap = 8;
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!cache) return 0;   /* allocation failure fallback */
        cache_size = init_cap;

        /* initialize available first terms */
        cache[0] = 0;   /* a(0) */
        cache[1] = 0;   /* a(1) */
        cache[2] = 1;   /* a(2) */
        cache[3] = 0;   /* a(3) */
        cache[4] = 0;   /* a(4) */
        cache[5] = 1;   /* a(5) */
        cache[6] = 2;   /* a(6) */
    }

    /* If n is below cached size and computed, just return it */
    if (n < cache_size && n < 7) {
        return cache[n];
    }

    /* Ensure cache large enough to hold values up to n */
    if ((size_t)n >= cache_size) {
        size_t new_cap = cache_size * 2;
        if (new_cap == 0) new_cap = 8;
        while (new_cap <= (size_t)n) new_cap *= 2;

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;   /* allocation failure fallback */
        cache = tmp;
        cache_size = new_cap;
    }

    /* Compute missing values up to n */
    for (size_t i = 7; i <= (size_t)n; ++i) {
        seq_t v = cache[i - 2] + 2 * cache[i - 4] - cache[i - 6];
        cache[i] = v;
    }

    return cache[n];
}
