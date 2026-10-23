/*
 * A042660  Numerators of continued fraction convergents to sqrt(860).
 * https://oeis.org/A042660
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 7742*a(n-4) - a(n-8); a(0)=29, a(1)=88, a(2)=1261,
 *            a(3)=3871, a(4)=225779, a(5)=681208, a(6)=9762691, a(7)=29969281
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Persistent cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static const seq_t init[8] = {
        (seq_t)29,
        (seq_t)88,
        (seq_t)1261,
        (seq_t)3871,
        (seq_t)225779,
        (seq_t)681208,
        (seq_t)9762691,
        (seq_t)29969281
    };

    /* Initialise cache with the first 8 terms on first call */
    if (cache == NULL) {
        cache = (seq_t *)malloc(8 * sizeof(seq_t));
        for (int i = 0; i < 8; ++i) cache[i] = init[i];
        cache_size = 8;
    }

    /* If the requested term is already in the cache, return it */
    if (n < cache_size) {
        return cache[(size_t)n];
    }

    /* Extend the cache up to the requested index n */
    size_t new_size = (size_t)(n + 1);
    cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));

    for (size_t i = cache_size; i < new_size; ++i) {
        /* a(i) = 7742 * a(i-4) - a(i-8) */
        cache[i] = (seq_t)7742 * cache[i - 4] - cache[i - 8];
    }

    cache_size = new_size;
    return cache[(size_t)n];
}
