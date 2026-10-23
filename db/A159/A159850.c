/*
 * A159850  Numerator of Hermite(n, 17/22).
 * https://oeis.org/A159850
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 17*a(n-1) + 242*(1-n)*a(n-2), a(0)=1, a(1)=17
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 7, i64 n <= 12, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoised values */
    static seq_t *cache = NULL;   /* array of seq_t values */
    static idx_t cache_size = 0;  /* number of values currently stored */

    /* initialise cache with first two terms */
    if (cache_size == 0) {
        cache_size = 2;
        cache = (seq_t *)calloc(cache_size, sizeof(seq_t));
        if (!cache) return 0;          /* allocation failure */
        cache[0] = (seq_t)1;           /* a(0) */
        cache[1] = (seq_t)17;          /* a(1) */
    }

    /* if requested term already cached, return it */
    if (n < cache_size) {
        return cache[n];
    }

    /* remember current cache size (old number of stored terms) */
    idx_t old_size = cache_size;

    /* grow cache to accommodate a(n) */
    seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!new_cache) return 0;  /* allocation failure */
    cache = new_cache;
    cache_size = n + 1;

    /* compute missing terms using the recurrence */
    for (idx_t i = old_size; i <= n; ++i) {
        seq_t term = (seq_t)17 * cache[i - 1]
                   + (seq_t)242 * ((seq_t)1 - (seq_t)i) * cache[i - 2];
        cache[i] = term;
    }

    return cache[n];
}
