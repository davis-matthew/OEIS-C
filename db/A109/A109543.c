/*
 * A109543  a(n) = a(n-1) + a(n-3) + a(n-5), with a(1..5) = 1.
 * https://oeis.org/A109543
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 27, u32 n <= 51, u64 n <= 101,
 *            u128 n <= 199, big any n
 * Helpers:   resize_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) + a(n-3) + a(n-5), a(0) = … = a(4) = 1 */

#include <stdlib.h>
#include <string.h>
#include "../utils/resize_cache.h"

static seq_t *cache = NULL;     /* stored terms */
static idx_t cache_size = 0;    /* allocated array size */
static idx_t comp = 4;          /* last computed index */

seq_t a(idx_t n)
{
    /* base values */
    if (n < 5)
        return (seq_t)1;

    /* initialize cache on first call */
    if (cache_size == 0) {
        cache_size = 5;
        cache = resize_cache(NULL, 0, cache_size);
        for (idx_t i = 0; i < cache_size; ++i)
            cache[i] = (seq_t)1;
        comp = 4;
    }

    /* grow cache if necessary */
    if (n >= cache_size) {
        idx_t new_size = cache_size * 2;
        while (new_size <= n)
            new_size *= 2;
        cache = resize_cache(cache, cache_size, new_size);
        cache_size = new_size;
        for (idx_t i = comp + 1; i < cache_size; ++i) {
            if (i < 5)
                cache[i] = (seq_t)1;
            else
                cache[i] = (seq_t)0;  /* placeholder */
        }
    }

    /* compute missing terms up to n */
    for (idx_t i = comp + 1; i <= n; ++i)
        cache[i] = cache[i - 1] + cache[i - 3] + cache[i - 5];

    comp = n;
    return cache[n];
}
