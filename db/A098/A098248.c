/*
 * A098248  Chebyshev polynomials S(n,291).
 * https://oeis.org/A098248
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=291; a(n)=291*a(n-1)-a(n-2) (n≥2)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>    /* for malloc, realloc */

seq_t a(idx_t n)
{
    /* static cache to keep previously computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of cached terms */

    /* Ensure scratch memory exists for all terms up to n */
    if (n >= cache_size) {
        /* Grow cache to accommodate indices 0…n */
        size_t new_size = (size_t)(n + 1);
        seq_t *new_cache = cache ? (seq_t *)realloc(cache, new_size * sizeof(seq_t))
                                 : (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!new_cache) {
            /* memory allocation failed – abort with zero (undefined behaviour) */
            return (seq_t)0;
        }
        cache = new_cache;

        /* Initialise newly added entries */
        if (cache_size == 0) {
            if (n >= 0) cache[0] = (seq_t)1;
            if (n >= 1) cache[1] = (seq_t)291;
            cache_size = (idx_t)2;
        } else if (cache_size == 1) {
            cache[1] = (seq_t)291;
            cache_size = (idx_t)2;
        }

        /* Compute missing terms up to n */
        for (idx_t i = cache_size; i <= n; ++i) {
            /* a(i) = 291*a(i-1) - a(i-2) */
            cache[i] = (seq_t)291 * cache[i - 1] - cache[i - 2];
        }
        cache_size = (idx_t)(n + 1);
    }

    return cache[n];
}
