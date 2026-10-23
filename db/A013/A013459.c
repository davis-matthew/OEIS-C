/*
 * A013459  Expansion of e.g.f. exp(arctan(x) - log(x+1)).
 * https://oeis.org/A013459
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   (n+1)^2*(n+2)*a(n)+n*(n+2)*a(n+1)+(n+2)*a(n+2)+a(n+3)=0, initial
 *            values: a(0)=1, a(1)=0, a(2)=1, a(3)=-4
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 12, i64 n <= 20, i128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;      /* array of computed terms */
    static size_t  cache_cap = 0;    /* allocated capacity */
    static idx_t   computed = 0;     /* highest index already computed */

    /* make sure we have room for the requested index */
    if (cache_cap == 0) {
        cache_cap = 4;                       /* at least indices 0..3 */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();                 /* allocation failed */
        /* initialise first four terms */
        cache[0] = 1;
        cache[1] = 0;
        cache[2] = 1;
        cache[3] = -4;
        computed = 3;
    } else if ((size_t)n + 4 > cache_cap) {
        size_t new_cap = (size_t)n + 4;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
        /* new slots are uninitialised – they will be filled below */
    }

    /* if we already know the answer, return it */
    if (n <= computed) return cache[n];

    /* compute missing terms up to n */
    for (idx_t m = computed + 1; m <= n; ++m) {
        idx_t m3 = m - 3;
        idx_t m2 = m - 2;
        idx_t m1 = m - 1;

        seq_t term1 = (seq_t)(m - 2) * (seq_t)(m - 2) * (seq_t)(m - 1) * cache[m3];
        seq_t term2 = (seq_t)(m - 3) * (seq_t)(m - 1) * cache[m2];
        seq_t term3 = (seq_t)(m - 1) * cache[m1];

        cache[m] = -(term1 + term2 + term3);
    }
    computed = n;
    return cache[n];
}
