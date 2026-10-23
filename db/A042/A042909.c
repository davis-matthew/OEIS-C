/*
 * A042909  Denominators of continued fraction convergents to sqrt(986).
 * https://oeis.org/A042909
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 314*a(n-3) + a(n-6) for n > 5 Initial values: a(0)=1,
 *            a(1)=2, a(2)=5, a(3)=312, a(4)=629, a(5)=1570
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[6] = {1, 2, 5, 312, 629, 1570};

static seq_t *cache = NULL;   /* stores values a(0) ... a(k) for some k */
static size_t cache_cap = 0;  /* number of elements stored in cache */

seq_t a(idx_t n)
{
    /* Return one of the six fixed initial values directly. */
    if (n < 6)
        return init_vals[(size_t)n];

    /* Initialise cache the first time we need a value beyond the sixth. */
    if (cache_cap == 0) {
        cache = (seq_t*)calloc(6, sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        for (size_t i = 0; i < 6; ++i)
            cache[i] = init_vals[i];
        cache_cap = 6;
    }

    /* Extend the cache to include the requested index n. */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            free(cache);
            exit(EXIT_FAILURE);
        }
        cache = tmp;
        /* Compute missing terms using the recurrence. */
        for (size_t i = cache_cap; i < new_cap; ++i)
            cache[i] = 314 * cache[i - 3] + cache[i - 6];
        cache_cap = new_cap;
    }

    return cache[(size_t)n];
}
