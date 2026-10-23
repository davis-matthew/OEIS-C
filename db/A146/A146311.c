/*
 * A146311  a(n) = cos(2*n*arcsin(sqrt(3))) =
 *          (-1)^n*cosh(2*n*arcsinh(sqrt(2))).
 * https://oeis.org/A146311
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -10*a(n-1) - a(n-2); a(0)=1, a(1)=-5
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 9, i64 n <= 19, i128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* memoisation cache */
    static size_t cache_size = 0; /* currently cached size */

    /* If already computed, return it immediately */
    if ((size_t)n < cache_size)
        return cache[(size_t)n];

    /* Compute up to index n, extending the cache if necessary */
    size_t new_size = (size_t)n + 1;
    size_t new_cap  = new_size;

    if (cache == NULL) {
        cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (!cache) return 0;            /* out‑of‑memory: return 0 as sentinel */
    } else if (new_cap > cache_size) {
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!cache) return 0;            /* out‑of‑memory */
    }

    /* Fill the newly needed entries of the cache */
    for (size_t i = cache_size; i < new_size; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
        } else if (i == 1) {
            cache[1] = (seq_t)-5;
        } else {
            cache[i] = (seq_t)(-10) * cache[i-1] - cache[i-2];
        }
    }

    cache_size = new_size;
    return cache[(size_t)n];
}
