/*
 * A176959  Sequence defined by the recurrence formula
 *          a(n+1)=sum(a(p)*a(n-p)+k,p=0..n)+l for n>=1, with here a(0)=1,
 *          a(1)=0, k=-1 and l=1.
 * https://oeis.org/A176959
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(0)=1, a(1)=0; for n>=1: a(n+1)=sum_{p=0}^{n}
 *            (a(p)*a(n-p)+k)+l, with k=-1 and l=1. In equational form: a(n+1)
 *            = sum_{p=0}^{n} a(p)*a(n-p) - n
 * Range:     i8 n <= 8, i16 n <= 14, i32 n <= 23, i64 n <= 28, i128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes needed for malloc/realloc.  No other functions are required. */
#include <stdlib.h>

/* seq_t and idx_t are provided by the build system. */
seq_t a(idx_t n)
{
    /* Static cache: keep previously computed values for all future calls. */
    static seq_t *cache = NULL;     /* array of computed a(i) values   */
    static size_t cache_cap = 0;    /* allocated capacity (in seq_t units) */
    static idx_t  cache_max = -1;   /* index of highest computed element   */

    /* Initialisation: create space for a(0) and a(1). */
    if (cache == NULL) {
        cache_cap = 2;                         /* at least 0 and 1 */
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();                   /* allocation error */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache_max = 1;                         /* last computed index */
    }

    /* If requested n is already known, just return it. */
    if (n <= cache_max) {
        return cache[(size_t)n];
    }

    /* Ensure there is enough space for all needed indices. */
    if ((size_t)(n+1) > cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)(n+1) > new_cap)
            new_cap <<= 1;                     /* double until large enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* Compute all new values from cache_max+1 up to n. */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        idx_t k = i - 1;                        /* we need S(k) for a(i) */
        seq_t s = 0;
        for (idx_t p = 0; p <= k; ++p) {
            s += cache[(size_t)p] * cache[(size_t)(k - p)];
        }
        cache[(size_t)i] = s - (seq_t)k;         /* a(i) = S(k) - k     */
    }

    cache_max = n;                               /* update highest index */

    return cache[(size_t)n];
}
