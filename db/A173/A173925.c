/*
 * A173925  Expansion of 1/(1 - x - x^8 - x^15 + x^16).
 * https://oeis.org/A173925
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-8) + a(n-15) - a(n-16) Initial values:
 *            a(0)=1, a(1)=1, a(2)=1, a(3)=1, a(4)=1, a(5)=1, a(6)=1, a(7)=1,
 *            a(8)=2, a(9)=3, a(10)=4, a(11)=5, a(12)=6, a(13)=7, a(14)=8,
 *            a(15)=10
 * Range:     u8 n <= 30, u16 n <= 56, u32 n <= 108, u64 n <= 213,
 *            u128 n <= 422, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[16] = {
    1, 1, 1, 1, 1, 1, 1, 1,
    2, 3, 4, 5, 6, 7, 8, 10
};

static seq_t *cache = NULL;
static size_t cache_size = 0;

seq_t a(idx_t n)
{
    size_t sn = (size_t)n;

    /* Direct lookup for the first 16 terms. */
    if (sn < 16) {
        return init_vals[sn];
    }

    /* If the cache has never been created, allocate it up to sn. */
    if (cache == NULL) {
        cache_size = sn + 1;
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        if (!cache) abort(); /* allocation failed */

        /* Copy the known initial segment. */
        for (size_t i = 0; i < 16; ++i) {
            cache[i] = init_vals[i];
        }

        /* Compute the remaining terms up to sn. */
        for (size_t i = 16; i <= sn; ++i) {
            cache[i] = cache[i-1] + cache[i-8] + cache[i-15] - cache[i-16];
        }
        return cache[sn];
    }

    /* If the requested value is already inside the cache, return it. */
    if (sn < cache_size) {
        return cache[sn];
    }

    /* The cache must be enlarged. */
    size_t old_size = cache_size;
    cache_size = sn + 1;
    seq_t *tmp = (seq_t*)realloc(cache, cache_size * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;

    /* Compute new terms that are now within the cache. */
    for (size_t i = old_size; i <= sn; ++i) {
        cache[i] = cache[i-1] + cache[i-8] + cache[i-15] - cache[i-16];
    }

    return cache[sn];
}
