/*
 * A218992  Power ceiling sequence of 3+sqrt(10).
 * https://oeis.org/A218992
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - 5*a(n-2) - a(n-3) a(0) = 7, a(1) = 44, a(2) =
 *            272 (equivalent to a(n) = ceil((3+sqrt(10))*a(n-1)) for the
 *            integer type with enough precision)
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;      /* dynamically allocated cache */
    static size_t cache_len = 0;     /* number of terms already stored */
    static size_t cache_cap = 0;     /* allocated capacity */

    /* Return cached value if already computed */
    if (n < cache_len)
        return cache[n];

    /* Grow the cache if necessary */
    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 8;
        while (new_cap < needed)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) { /* handle allocation failure */
            /* In typical judge environments, aborting is acceptable */
            abort();
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise base cases lazily */
    if (cache_len == 0) {
        cache[0] = (seq_t)7;
        cache_len = 1;
    }
    if (cache_len == 1) {
        cache[1] = (seq_t)44;
        cache_len = 2;
    }
    if (cache_len == 2) {
        cache[2] = (seq_t)272;
        cache_len = 3;
    }

    /* Compute missing terms up to n */
    for (size_t i = cache_len; i < needed; ++i) {
        /* Use the recurrence a(i) = 7*a(i-1) - 5*a(i-2) - a(i-3) */
        cache[i] = (seq_t)(cache[i - 1] * (seq_t)7
                           - cache[i - 2] * (seq_t)5
                           - cache[i - 3]);
    }

    cache_len = needed;
    return cache[n];
}
