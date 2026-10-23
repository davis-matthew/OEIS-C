/*
 * A042264  Numerators of continued fraction convergents to sqrt(658).
 * https://oeis.org/A042264
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 3386*a(n-8) - a(n-16), a(0) = 25, a(1) = 26, a(2) = 51, *
 *            a(3) = 77, a(4) = 513, a(5) = 590, a(6) = 1103, a(7) = 1693, *
 *            a(8) = 85753, a(9) = 87446, a(10) = 173199, a(11) = 260645, *
 *            a(12) = 1737069, a(13) = 1997714, a(14) = 3734783, a(15) =
 *            5732497
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 19, u64 n <= 41, u128 n <= 87,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/mul_small.h"

/* initial values (seed for the recurrence) */
static const seq_t init[16] = {
    25, 26, 51, 77, 513, 590, 1103, 1693,
    85753, 87446, 173199, 260645, 1737069,
    1997714, 3734783, 5732497
};

seq_t a(idx_t n)
{
    /* For indices below 16, the values are known directly */
    if (n < 16u)
        return init[(size_t)n];

    /* cache to store previously computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated capacity */
    static size_t cache_len = 0;   /* number of elements already written */

    /* Ensure capacity for needed indices */
    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 64;
        while (new_cap < needed)
            new_cap <<= 1;
        seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_ptr)   /* allocation failed – abort gracefully */
            abort();
        cache = new_ptr;
        cache_cap = new_cap;
    }

    /* initialise the first 16 entries if not already done */
    if (cache_len < 16) {
        for (size_t i = cache_len; i < 16; ++i)
            cache[i] = init[i];
        cache_len = 16;
    }

    /* compute any missing terms using the recurrence relation */
    if (cache_len < needed) {
        for (size_t i = cache_len; i < needed; ++i) {
            seq_t temp = mul_small(cache[i - 8], 3386u);
            cache[i] = temp - cache[i - 16];
        }
        cache_len = needed;
    }

    return cache[(size_t)n];
}
