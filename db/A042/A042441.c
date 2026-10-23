/*
 * A042441  Denominators of continued fraction convergents to sqrt(748).
 * https://oeis.org/A042441
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 11316494*a(n-12) - a(n-24) for n > 23 Initial values
 *            (a(0) … a(23)): 1, 2, 3, 20, 103, 123, 1579, 1702, 10089, 62236,
 *            72325, 206886, 11244169, 22695224, 33939393, 226331582,
 *            1165597303, 1391928885, 17868743923, 19260672808, 114172107963,
 *            704293320586, 818465428549, 2341224177684
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 34, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* Array of the first 24 terms */
    static const seq_t init[24] = {
        1ULL, 2ULL, 3ULL, 20ULL, 103ULL, 123ULL, 1579ULL, 1702ULL,
        10089ULL, 62236ULL, 72325ULL, 206886ULL, 11244169ULL, 22695224ULL,
        33939393ULL, 226331582ULL, 1165597303ULL, 1391928885ULL,
        17868743923ULL, 19260672808ULL, 114172107963ULL, 704293320586ULL,
        818465428549ULL, 2341224177684ULL
    };

    /* Static cache for computed values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;

    /* If the requested n is within the initial values */
    if (n < 24) {
        return init[(size_t)n];
    }

    /* Allocate or resize the cache on first use */
    if (cache == NULL) {
        cache_cap = 24;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0; /* fallback on allocation failure */
        for (size_t i = 0; i < 24; ++i) {
            cache[i] = init[i];
        }
    }

    /* Ensure the cache is large enough */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1; /* double until sufficient */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0; /* fallback on allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n using the recurrence */
    size_t next = 24;
    while (next <= (size_t)n) {
        /* a(next) = 11316494 * a(next - 12) - a(next - 24) */
        cache[next] = (11316494 * cache[next - 12]) - cache[next - 24];
        ++next;
    }

    return cache[(size_t)n];
}
