/*
 * A195622  Denominators of Pythagorean approximations to 5.
 * https://oeis.org/A195622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = 101*a(n-1) + 101*a(n-2) - a(n-3) Initial values: a(1) =
 *            20, a(2) = 2020, a(3) = 206040
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* allocated capacity of cache      */
static size_t cache_len = 0;   /* number of sequence values stored */

/* Ensure the cache is large enough to hold at least `mincap` elements */
static void grow_cache(size_t mincap) {
    if (mincap <= cache_cap) return;
    size_t newcap = cache_cap ? cache_cap * 2 : 8;
    while (newcap < mincap) newcap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    cache = tmp;
    cache_cap = newcap;
}

/* Initialise the first three terms of the sequence */
static void init_cache(void) {
    cache[0] = 20;
    cache[1] = 2020;
    cache[2] = 206040;
    cache_len = 3;
}

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;

    if (cache == NULL) {
        grow_cache(3);
        init_cache();
    }

    /* If we need more space, grow the cache */
    if ((size_t)n > cache_cap) grow_cache((size_t)n);

    size_t idx = (size_t)n - 1;   /* zero‑based index in cache   */
    if (idx < cache_len) return cache[idx];

    /* Compute missing terms up to n using the recurrence */
    for (size_t i = cache_len; i <= idx; ++i) {
        seq_t val = 101 * cache[i - 1] + 101 * cache[i - 2] - cache[i - 3];
        cache[i] = val;
    }

    cache_len = idx + 1;
    return cache[idx];
}
