/*
 * A099110  Expansion of 1 / ((1+x)*(1-2x)*(1-3x)*(1-4x)).
 * https://oeis.org/A099110
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=8, a(2)=47, a(3)=238,
   a(n)=8*a(n-1)-17*a(n-2)-2*a(n-3)+24*a(n-4) for n>=4 */
/* The implementation memoises results in a static array. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache   = NULL;   /* holds the sequence values */
    static idx_t cache_len = 0;     /* next index to compute */
    static size_t cache_cap = 0;    /* allocated capacity */

    /* initial terms */
    static const seq_t init[4] = {
        (seq_t)1,   /* a(0) */
        (seq_t)8,   /* a(1) */
        (seq_t)47,  /* a(2) */
        (seq_t)238  /* a(3) */
    };

    /* trivial case */
    if (n < 4)
        return init[n];

    /* ensure the cache is allocated */
    if (cache_len == 0) {
        /* initial allocation at least up to n - but keep a small initial size */
        cache_cap = 8 > (size_t)n ? 8 : (size_t)n + 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;  /* out of memory (unlikely) */
        for (idx_t i = 0; i < 4; ++i)
            cache[i] = init[i];
        cache_len = 4;
    }
    else if ((idx_t)cache_cap <= n) {
        /* grow capacity (double strategy) */
        size_t new_cap = cache_cap * 2;
        if (new_cap <= (size_t)n)
            new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t a0 = cache[i-4];
        seq_t a1 = cache[i-3];
        seq_t a2 = cache[i-2];
        seq_t a3 = cache[i-1];
        /* compute 8*a3 - 17*a2 - 2*a1 + 24*a0 */
        /* to avoid unsigned underflow, compute as sum1 - sum2 */
        seq_t sum1 = a3 * 8 + a0 * 24;
        seq_t sum2 = a2 * 17 + a1 * 2;
        cache[i] = sum1 - sum2;
    }
    cache_len = n + 1;

    return cache[n];
}
