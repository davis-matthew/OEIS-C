/*
 * A008807  Expansion of (1+x^5)/((1-x^2)^2*(1-x^5)).
 * https://oeis.org/A008807
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A008807:
 *  G.f. = (1+x^5)/((1-x^2)^2*(1-x^5))
 *  Recurrence: a(n) = 2*a(n-2) - a(n-4) + a(n-5) - 2*a(n-7) + a(n-9)  (n>=9)
 *  Initial values:
 *    a(0)=1, a(1)=0, a(2)=2, a(3)=0, a(4)=3, a(5)=2,
 *    a(6)=4, a(7)=4, a(8)=5
 */
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   /* number of allocated elements         */
    static idx_t cache_max  = (idx_t)-1;  /* last computed index (wrap around error handled) */

    /* initialize cache on first call */
    if (cache == NULL) {
        /* start with capacity sufficient for the first 9 terms */
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;  /* allocation failure (should not happen) */

        /* seed initial 9 terms */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)0;
        cache[4] = (seq_t)3;
        cache[5] = (seq_t)2;
        cache[6] = (seq_t)4;
        cache[7] = (seq_t)4;
        cache[8] = (seq_t)5;

        cache_max = 8;
    }

    /* if requested index is already computed, return it */
    if (n <= cache_max) return cache[n];

    /* ensure cache has enough capacity */
    if (n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;  /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute terms up to n */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        /* recurrence: a(i) = 2*a(i-2) - a(i-4) + a(i-5) - 2*a(i-7) + a(i-9)   (i>=9) */
        seq_t t1 = (cache[i - 2] << 1) + cache[i - 5] + cache[i - 9];
        seq_t t2 = cache[i - 4] + (cache[i - 7] << 1);
        cache[i] = t1 - t2;
    }

    cache_max = n;
    return cache[n];
}
