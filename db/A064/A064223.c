/*
 * A064223  a(1) = 1; a(n+1) = a(n) + number of decimal digits of a(n) for n >
 *          0.
 * https://oeis.org/A064223
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, nonn, base
 * Formula:   a(1) = 1; a(n+1) = a(n) + number_of_decimal_digits(a(n))
 * Range:     u8 n <= 106, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_count_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/digit_count_seq.h"

/* Compute a(n) according to A064223. */
seq_t a(idx_t n) {
    /* Static cache for memoisation. */
    static idx_t cache_len = 0;      /* number of elements stored */
    static size_t cache_cap = 0;     /* allocated capacity */
    static seq_t *cache = NULL;      /* array */

    if (n == 0) {        /* out of bounds; not defined by OEIS */
        return (seq_t)0;
    }

    /* First call – initialise cache with a(1) */
    if (cache_len == 0) {
        cache_cap = 1;
        cache = (seq_t*)malloc(sizeof(seq_t));
        if (!cache) { /* allocation failed – return 0 as a fallback */
            return (seq_t)0;
        }
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* If requested term is already cached, return it. */
    if (cache_len >= n) {
        return cache[n - 1];
    }

    /* Grow the cache if necessary. */
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap == 0 ? 16 : cache_cap;
        while (new_cap < (size_t)n) {
            new_cap <<= 1;            /* double until sufficient */
        }
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) { /* realloc failed – return last computed value */
            return cache[cache_len - 1];
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n. */
    for (idx_t i = cache_len; i < n; ++i) {
        seq_t prev = cache[i - 1];
        int d = digit_count_seq(prev);
        cache[i] = prev + (seq_t)d;
    }
    cache_len = n;

    return cache[n - 1];
}
