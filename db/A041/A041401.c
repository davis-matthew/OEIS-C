/*
 * A041401  Denominators of continued fraction convergents to sqrt(215).
 * https://oeis.org/A041401
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 88*a(n-4) - a(n-8) for n > 7; a(0) = 1, a(1) = 1, a(2) =
 *            2, a(3) = 3, a(4) = 86, a(5) = 89, a(6) = 175, a(7) = 264
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 19, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static const seq_t _base_vals[8] = {
    (seq_t)1,
    (seq_t)1,
    (seq_t)2,
    (seq_t)3,
    (seq_t)86,
    (seq_t)89,
    (seq_t)175,
    (seq_t)264
};

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;

    /* Ensure the cache is large enough to hold a(n) */
    size_t required = (size_t)n + 1;
    if (required < 8) required = 8;
    if (required > cap) {
        seq_t *new_cache = (seq_t *)realloc(cache, required * sizeof(seq_t));
        if (!new_cache) return 0;          /* allocation failure -> fallback */
        cache = new_cache;
        for (idx_t i = (idx_t)cap; i < (idx_t)required; ++i) {
            if (i < 8) {
                cache[i] = _base_vals[i];
            } else {
                cache[i] = (seq_t)88 * cache[i - 4] - cache[i - 8];
            }
        }
        cap = required;
    }
    return cache[n];
}
