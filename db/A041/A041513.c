/*
 * A041513  Denominators of continued fraction convergents to sqrt(273).
 * https://oeis.org/A041513
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 1454*a(n-6) - a(n-12) for n > 11, with initial terms a(0)
 *            = 1, a(1) = 1, a(2) = 2, a(3) = 21, a(4) = 23, a(5) = 44, a(6) =
 *            1431, a(7) = 1475, a(8) = 2906, a(9) = 30535, a(10) = 33441,
 *            a(11) = 63976
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 19, u64 n <= 37, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

static void ensure_cache(idx_t n)
{
    /* Ensure array capacity and fill any missing terms up to n. */
    size_t needed = (size_t)n + 1;

    /* Initialize base terms on first use. */
    if (cache_len == 0) {
        size_t init_cap = 16;
        if (needed > init_cap) init_cap = needed;
        cache = (seq_t*)malloc(init_cap * sizeof(seq_t));
        cache_cap = init_cap;

        cache[0] = 1; cache[1] = 1; cache[2] = 2; cache[3] = 21;
        cache[4] = 23; cache[5] = 44; cache[6] = 1431;
        cache[7] = 1475; cache[8] = 2906; cache[9] = 30535;
        cache[10] = 33441; cache[11] = 63976;
        cache_len = 12;
    }

    /* Grow array if necessary. */
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 16;
        while (new_cap < needed) new_cap <<= 1;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute missing terms using the recurrence. */
    for (size_t i = cache_len; i < needed; ++i) {
        if (i >= 12) {
            cache[i] = ((seq_t)1454 * cache[i-6]) - cache[i-12];
        }
    }
    cache_len = needed;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
