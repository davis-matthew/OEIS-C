/*
 * A042180  Numerators of continued fraction convergents to sqrt(615).
 * https://oeis.org/A042180
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(n) = 248*a(n-4) - a(n-8) Initial values: a(0)=24, a(1)=25,
 *            a(2)=99, a(3)=124, a(4)=6051, a(5)=6175, a(6)=24576, a(7)=30751
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static const seq_t init_vals[8] = {
    24, 25, 99, 124, 6051, 6175, 24576, 30751
};

seq_t a(idx_t n)
{
    static seq_t *cache        = NULL; /* memoization storage */
    static size_t cache_cap    = 0;     /* heap capacity (in elements) */
    static size_t cache_len    = 0;     /* number of elements stored */

    size_t idx = (size_t)n;

    /* enlarge cache if needed */
    if (idx >= cache_cap) {
        size_t new_cap = cache_cap > 0 ? cache_cap << 1 : 16;
        while (new_cap <= idx) new_cap <<= 1;

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* allocation failure – return zero (undefined) */
            return (seq_t)0;
        }
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* compute terms that are not yet memoized */
    for (size_t i = cache_len; i <= idx; ++i) {
        if (i < 8) {
            cache[i] = init_vals[i];
        } else {
            cache[i] = (seq_t)248 * cache[i - 4] - cache[i - 8];
        }
    }
    cache_len = idx + 1;

    return cache[idx];
}
