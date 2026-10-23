/*
 * A041415  Denominators of continued fraction convergents to sqrt(222).
 * https://oeis.org/A041415
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 298*a(n-4) - a(n-8); initial values
 *            a(0)=1,a(1)=1,a(2)=9,a(3)=10,a(4)=289,a(5)=299,
 *            a(6)=2681,a(7)=2980
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[8] = {
    1, 1, 9, 10, 289, 299, 2681, 2980
};

static seq_t *cache = NULL;
static idx_t cache_size = 0;

/* Ensure that the cache contains the first eight terms. */
static void init_cache(void)
{
    if (cache != NULL) return;
    cache_size = 8;
    cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
    if (!cache) exit(EXIT_FAILURE);
    for (idx_t i = 0; i < 8; ++i) cache[i] = init_vals[i];
}

/* Grow the cache so that it contains at least terms 0 .. n. */
static void extend_cache(idx_t n)
{
    if (n < cache_size) return;
    idx_t new_size = n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) exit(EXIT_FAILURE);
    cache = new_cache;
    for (idx_t i = cache_size; i <= n; ++i) {
        cache[i] = (seq_t)298 * cache[i-4] - cache[i-8];
    }
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    if (n < 8) return init_vals[n];
    init_cache();
    if (n >= cache_size) extend_cache(n);
    return cache[n];
}
