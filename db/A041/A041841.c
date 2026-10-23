/*
 * A041841  Denominators of continued fraction convergents to sqrt(442).
 * https://oeis.org/A041841
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 42*a(n-1) + a(n-2), a(0)=1, a(1)=42
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t computed = 0; /* last computed index */

    /* initialise first two terms on first call */
    if (!cache) {
        cache = (seq_t*)malloc(2 * sizeof(seq_t));
        if (!cache) abort();
#ifndef __cplusplus
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)42;
#else
        new(&cache[0]) seq_t();
        new(&cache[1]) seq_t();
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)42;
#endif
        computed = 1;
    }

    /* if requested value already known, return it */
    if (n <= computed) {
        return cache[n];
    }

    /* grow cache to accommodate n */
    seq_t *tmp = (seq_t*)realloc(cache,
                           (size_t)(n + 1) * sizeof(seq_t));
    if (!tmp) abort();
#ifndef __cplusplus
    /* nothing to construct: built‑in types */
#else
    /* construct newly allocated elements */
    for (idx_t k = computed + 1; k <= n; ++k) {
        new(&tmp[k]) seq_t();
    }
#endif
    cache = tmp;

    /* compute missing terms using recurrence */
    for (idx_t i = computed + 1; i <= n; ++i) {
        cache[i] = (seq_t)42 * cache[i-1] + cache[i-2];
    }
    computed = n;

    return cache[n];
}
