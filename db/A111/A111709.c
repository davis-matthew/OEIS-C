/*
 * A111709  Expansion of (x^8-x^7+x^5-x+1)*(x^4+1) /
 *          ((x+1)*(x^2+1)*(x^4-x^2+1)*(x^2-x+1)*(x^2+x+1)*(x-1)^2).
 * https://oeis.org/A111709
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-12) - a(n-13) for n > 12, with a(0) = 1,
 *            a(1) = 0, a(2) = 0, a(3) = 0, a(4) = 1, a(5) = 1, a(6) = 1, a(7)
 *            = 0, a(8) = 1, a(9) = 2, a(10) = 2, a(11) = 1, a(12) = 3
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static const seq_t init_vals[13] = {
    1, 0, 0, 0, 1, 1, 1, 0, 1, 2, 2, 1, 3
};

static seq_t *cache = NULL;
static idx_t cache_size = 0;

/* Extend the cache so that indices up to n are available. */
static void ensure_cache(idx_t n)
{
    /* Ensure base values are stored. */
    if (cache_size < 13) {
        cache = (seq_t *)realloc(cache, 13 * sizeof(seq_t));
        for (idx_t i = 0; i < 13; ++i) {
            cache[i] = init_vals[i];
        }
        cache_size = 13;
    }

    /* Compute further elements if necessary. */
    if (n + 1 > cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        cache = tmp;
        for (idx_t i = cache_size; i <= n; ++i) {
            /* Recurrence: a(i) = a(i-1) + a(i-12) - a(i-13) */
            cache[i] = cache[i - 1] + cache[i - 12] - cache[i - 13];
        }
        cache_size = new_size;
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
