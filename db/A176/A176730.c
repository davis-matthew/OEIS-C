/*
 * A176730  Denominators of coefficients of a series, called f, related to
 *          Airy functions.
 * https://oeis.org/A176730
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 3*n*(3*n-1)*a(n-1), a(0)=1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache so calls in any order work */
    static seq_t *cache   = NULL;
    static size_t cached  = 0;         /* number of pre‑computed entries
                                          (cache[0 .. cached-1]) */

    /* extend cache if needed */
    if ((size_t)n >= cached) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();            /* allocation failed */
        cache = tmp;

        /* initialise a(0) if the cache is empty */
        if (cached == 0) {
            cache[0] = 1;
            cached   = 1;
        }

        /* compute missing terms */
        for (size_t i = cached; i <= (size_t)n; ++i) {
            /* mult = (3*i)*(3*i-1) as seq_t */
            seq_t mult = (seq_t)(3 * i) * (seq_t)(3 * i - 1);
            cache[i]   = cache[i - 1] * mult;
        }
        cached = new_size;
    }

    return cache[(size_t)n];
}
