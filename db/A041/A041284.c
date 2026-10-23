/*
 * A041284  Numerators of continued fraction convergents to sqrt(155).
 * https://oeis.org/A041284
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 498*a(n-4) - a(n-8)   (n ≥ 8)
 * Initial terms:
 *   a(0)=12, a(1)=25, a(2)=112, a(3)=249,
 *   a(4)=6088, a(5)=12425, a(6)=55788, a(7)=124001
 */
#include <stdlib.h>   /* malloc, realloc */

/* Computes the n-th term of A041284 */
seq_t a(idx_t n)
{
    /* static cache to store computed terms */
    static seq_t *cache     = NULL;  /* array of terms */
    static size_t cache_len = 0;     /* number of terms stored (>= cache_cap) */
    static size_t cache_cap = 0;     /* allocated capacity */

    /* fixed initial values for indices 0..7 */
    static const seq_t init[8] = {
        (seq_t)12,
        (seq_t)25,
        (seq_t)112,
        (seq_t)249,
        (seq_t)6088,
        (seq_t)12425,
        (seq_t)55788,
        (seq_t)124001
    };

    /* first call: allocate cache and fill with initial values */
    if (cache == NULL) {
        cache_cap = 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* on memory failure */
        for (size_t i = 0; i < 8; ++i)
            cache[i] = init[i];
        cache_len = 8;
    }

    /* if requested index is already computed, return it */
    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    /* extend cache capacity if necessary */
    size_t need = (size_t)n + 1;
    if (need > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, need * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* fallback on allocation failure */
        cache    = tmp;
        cache_cap = need;
    }

    /* compute missing terms using the recurrence */
    for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
        /* cache[i-4] and cache[i-8] are guaranteed to exist */
        cache[(size_t)i] = (seq_t)( ((seq_t)498 * cache[(size_t)(i-4)]) -
                                    cache[(size_t)(i-8)] );
    }
    cache_len = need;   /* now we have terms 0 .. n */

    return cache[(size_t)n];
}
