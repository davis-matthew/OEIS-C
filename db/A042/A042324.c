/*
 * A042324  Numerators of continued fraction convergents to sqrt(689).
 * https://oeis.org/A042324
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 210*a(n-2) - a(n-4) Initial values: a(0)=26, a(1)=105,
 *            a(2)=5486, a(3)=22049 The sequence is the numerators of the
 *            continued fraction convergents to sqrt(689)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_size = 0;

static void init_cache(void) {
    /* Allocate space for the first four known values */
    cache_size = 4;
    cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
    cache[0] = (seq_t)26;
    cache[1] = (seq_t)105;
    cache[2] = (seq_t)5486;
    cache[3] = (seq_t)22049;
}

seq_t a(idx_t n) {
    /* Ensure the cache is initialized */
    if (!cache) init_cache();

    /* Return directly if we already have the value */
    if ((size_t)n < cache_size)
        return cache[(size_t)n];

    /* Grow the cache to accommodate up to index n */
    size_t new_size = (size_t)n + 1;
    cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));

    /* Compute missing terms using the recurrence */
    for (size_t k = cache_size; k <= (size_t)n; ++k) {
        cache[k] = (seq_t)210 * cache[k - 2] - cache[k - 4];
    }

    cache_size = new_size;
    return cache[(size_t)n];
}
