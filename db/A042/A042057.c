/*
 * A042057  Denominators of continued fraction convergents to sqrt(552).
 * https://oeis.org/A042057
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 94*a(n-2) - a(n-4), a(0) = 1, a(1) = 2, a(2) = 93, a(3) =
 *            188
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static idx_t cached_last = 0;

    /* Initialise cache to hold the first four terms */
    if (n <= 3) {
        if (cache == NULL) {
            cache_size = 4;
            cache = (seq_t*)malloc(sizeof(seq_t) * cache_size);
            cache[0] = 1;
            cache[1] = 2;
            cache[2] = 93;
            cache[3] = 188;
            cached_last = 3;
        }
        return cache[n];
    }

    /* Ensure cache capacity */
    if (cache == NULL) {
        cache_size = 4;
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_size);
        cache[0] = 1;
        cache[1] = 2;
        cache[2] = 93;
        cache[3] = 188;
        cached_last = 3;
    }
    if ((idx_t)n + 1 > (idx_t)cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * new_size);
        if (tmp == NULL) {
            /* allocation failure – abort safely */
            abort();
        }
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute missing terms */
    for (idx_t k = cached_last + 1; k <= n; ++k) {
        cache[k] = 94 * cache[k - 2] - cache[k - 4];
    }
    cached_last = n;
    return cache[n];
}
