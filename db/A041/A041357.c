/*
 * A041357  Denominators of continued fraction convergents to sqrt(192).
 * https://oeis.org/A041357
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 194*a(n-4) - a(n-8) with initial values a(0)=1, a(1)=1,
 *            a(2)=6, a(3)=7, a(4)=188, a(5)=195, a(6)=1163, a(7)=1358
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 35, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n) {
    /* Base terms for n = 0..7 */
    static const seq_t base[8] = { 1, 1, 6, 7, 188, 195, 1163, 1358 };
    /* Cached values for n ≥ 0 */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n < 8) {
        return base[(size_t)n];
    }

    /* Allocate cache if necessary and fill base terms */
    if (cache == NULL) {
        cache_size = 8;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        for (size_t i = 0; i < 8; ++i) {
            cache[i] = base[i];
        }
    }

    /* Grow cache up to n if needed */
    if (n >= cache_size) {
        size_t old_size = (size_t)cache_size;
        cache_size = (idx_t)(n + 1);
        cache = (seq_t *)realloc(cache, cache_size * sizeof(seq_t));
        for (size_t i = old_size; i <= (size_t)n; ++i) {
            /* Sequence recurrence: a(n) = 194*a(n-4) - a(n-8) */
            cache[i] = ((seq_t)194 * cache[i - 4]) - cache[i - 8];
        }
    }

    return cache[(size_t)n];
}
