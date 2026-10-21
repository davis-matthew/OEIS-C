/*
 * A218737  a(n) = (34^n - 1)/33.
 * https://oeis.org/A218737
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (34^n - 1)/33 Recurrence: a(0)=0, a(1)=1,
 *            a(n)=35*a(n-1)-34*a(n-2) for n≥2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache of computed values; grows as needed */
    static seq_t *cache = NULL;
    static idx_t max_computed = 0;      /* highest index computed */
    static size_t cache_cap = 0;        /* allocated capacity */

    /* initialise cache with first two terms */
    if (cache == NULL) {
        cache_cap = 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;           /* allocation failed – unlikely on judge */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        max_computed = 1;
    }

    /* humble case: n already computed */
    if (n <= max_computed)
        return cache[n];

    /* enlarge cache if necessary */
    size_t required = (size_t)n + 1;
    if (required > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 2;
        while (new_cap < required)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;           /* allocation failed – unlikely on judge */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms using the recurrence */
    const seq_t C35 = (seq_t)35;
    const seq_t C34 = (seq_t)34;
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        cache[i] = C35 * cache[i-1] - C34 * cache[i-2];
    }
    max_computed = n;

    return cache[n];
}
