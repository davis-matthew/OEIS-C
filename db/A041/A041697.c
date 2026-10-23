/*
 * A041697  Denominators of continued fraction convergents to sqrt(368).
 * https://oeis.org/A041697
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 2302*a(n-4) - a(n-8) for n>7, with a(0)=1, a(1)=5,
 *            a(2)=11, a(3)=60, a(4)=2291, a(5)=11515, a(6)=25321, a(7)=138120
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

static const seq_t init_vals[8] = {
    1, 5, 11, 60, 2291, 11515, 25321, 138120
};

static seq_t *cache = NULL;          /* array of computed terms */
static size_t cache_size = 0;        /* number of elements stored */
static size_t cache_cap  = 0;        /* allocated capacity */

/* Ensure the cache holds at least n+1 elements. */
static void ensure_cache(idx_t n)
{
    if (cache_cap >= (size_t)(n + 1))
        return;

    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap <= (size_t)(n + 1))
        new_cap <<= 1;                /* double until enough */

    cache = seq_realloc(cache, cache_cap, new_cap);
    if (!cache)
        exit(EXIT_FAILURE);
    cache_cap = new_cap;

    /* Initialize the first 8 terms on first allocation. */
    if (cache_size == 0) {
        for (size_t i = 0; i < 8; ++i)
            cache[i] = init_vals[i];
        cache_size = 8;
    }
}

seq_t a(idx_t n)
{
    if (n < 8)
        return init_vals[(size_t)n];

    ensure_cache(n);

    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        /* recurrence: a(n) = 2302*a(n-4) - a(n-8) */
        cache[i] = (seq_t)2302 * cache[i - 4] - cache[i - 8];
    }
    cache_size = (size_t)n + 1;
    return cache[(size_t)n];
}
