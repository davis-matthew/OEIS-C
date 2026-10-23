/*
 * A108360  Expansion of (1-2x)^2/((1-x)^2(1-2x-x^2)^2).
 * https://oeis.org/A108360
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 6*a(n-1) - 11*a(n-2) + 4*a(n-3) + 5*a(n-4) - 2*a(n-5) -
 *            a(n-6) Initial values: a(0)=1, a(1)=2, a(2)=5, a(3)=12, a(4)=30,
 *            a(5)=76
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;   /* array of computed terms */
    static idx_t  cache_cap = 0;  /* allocated capacity */

    /* first six known terms (offset 0) */
    const seq_t init[6] = {
        (seq_t)1,  /* a(0) */
        (seq_t)2,  /* a(1) */
        (seq_t)5,  /* a(2) */
        (seq_t)12, /* a(3) */
        (seq_t)30, /* a(4) */
        (seq_t)76  /* a(5) */
    };

    /* ensure cache exists and has capacity at least 6 */
    if (cache_cap < 6) {
        seq_t *tmp = (seq_t *)realloc(cache, 6 * sizeof(seq_t));
        if (!tmp) return (seq_t)0;                 /* out of memory, unlikely */
        cache = tmp;
        cache_cap = 6;
        /* initialize the first six entries */
        for (idx_t i = 0; i < 6; ++i)
            cache[i] = init[i];
    }
    /* for n < 6 return the hard‑coded value */
    if (n < 6)
        return cache[n];

    /* grow cache capacity if needed */
    if (n + 1 > cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 8;
        if (new_cap < n + 1)
            new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;                 /* out of memory, unlikely */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = 6; i <= n; ++i) {
        /* recurrence: a(i) = 6*a(i-1)-11*a(i-2)+4*a(i-3)+5*a(i-4)-2*a(i-5)-a(i-6) */
        seq_t t = (seq_t)6 * cache[i-1];
        t -= (seq_t)11 * cache[i-2];
        t += (seq_t)4 * cache[i-3];
        t += (seq_t)5 * cache[i-4];
        t -= (seq_t)2 * cache[i-5];
        t -= cache[i-6];
        cache[i] = t;
    }

    return cache[n];
}
