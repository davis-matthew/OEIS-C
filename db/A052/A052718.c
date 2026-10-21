/*
 * A052718  Expansion of e.g.f. 1 - x - sqrt(1-4*x).
 * https://oeis.org/A052718
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=0, a(1)=1, a(2)=4; for n≥2: a(n+1) = (4*n-2)*a(n)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoized values */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;   /* current allocated capacity   */
    static size_t cache_size = 0;   /* number of values stored      */

    /* initialise cache with the first three values */
    if (cache == NULL) {
        cache_cap = 3;
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;       /* allocation failed (unlikely) */

        cache[0] = 0;
        cache[1] = 1;
        cache[2] = 4;
        cache_size = 3;
    }

    /* return value if already computed */
    if ((size_t)n < cache_size)
        return cache[n];

    /* grow cache if needed */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;               /* reallocation failed */
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        /* a(i) = (4*(i-1)-2) * a(i-1)   (for i≥3)   */
        seq_t factor = (seq_t)(4 * (i - 1) - 2);
        cache[i] = cache[i - 1] * factor;
    }
    cache_size = (size_t)n + 1;
    return cache[n];
}
