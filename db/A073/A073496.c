/*
 * A073496  Expansion of (3 + 2*x + 3*x^2)/(1 + x + 3*x^2 - x^3).
 * https://oeis.org/A073496
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-3) - 3*a(n-2) - a(n-1) Initial values: a(0)=3,
 *            a(1)=-1, a(2)=-5
 * Range:     i8 n <= 7, i16 n <= 17, i32 n <= 35, i64 n <= 36, i128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for computed terms.
       cache_capacity  – size of allocated array
       computed        – highest index that has a defined value in cache */
    static seq_t *cache = NULL;
    static idx_t cache_capacity = 0;
    static idx_t computed = 0;

    /* Initialise the first three terms on the first call. */
    if (cache == NULL) {
        cache_capacity = 3;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_capacity);
        if (!cache) return 0;        /* fallback, though not expected */
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)-1;
        cache[2] = (seq_t)-5;
        computed = 2;
    }

    /* If the requested index is already computed, return it. */
    if (n <= computed)
        return cache[n];

    /* Ensure the cache is large enough. */
    if (n + 1 > cache_capacity) {
        idx_t new_cap = cache_capacity;
        while (new_cap <= n)
            new_cap <<= 1;                  /* double until large enough */
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) return 0;                /* fallback */
        cache = tmp;
        cache_capacity = new_cap;
    }

    /* Compute missing terms up to index n. */
    for (idx_t i = computed + 1; i <= n; ++i) {
        cache[i] = cache[i-3] - 3*cache[i-2] - cache[i-1];
    }
    computed = n;

    return cache[n];
}
