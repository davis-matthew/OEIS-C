/*
 * A015589  Expansion of x/(1 - 10*x - 7*x^2).
 * https://oeis.org/A015589
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A015589: a(n) = 10*a(n-1) + 7*a(n-2),   a(0)=0, a(1)=1
   Offset: 0  */

#include <stddef.h>
#include <stdlib.h>

/* static cache of computed terms */
static seq_t  *cache      = NULL;   /* array of values */
static size_t  cache_cap  = 0;     /* allocated capacity (in elements) */
static size_t  cache_filled = 0;   /* number of entries already computed */

/* ensure that 'cache' can hold indices [0..n] (at least 2 slots) */
static void
ensure_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;      /* indices 0..n inclusive */
    if (needed < 2) needed = 2;         /* we must store a(0) and a(1) */

    if (needed <= cache_cap)
        return;                         /* already large enough */

    /* grow to next power of two >= needed */
    size_t new_cap = cache_cap ? cache_cap : 1;
    while (new_cap < needed)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                        /* unrecoverable, as per requirements */

    cache = tmp;
    cache_cap = new_cap;

    /* initialise any newly allocated slots to zero */
    for (size_t i = cache_filled; i < cache_cap; ++i)
        cache[i] = 0;
}

/* public sequence function */
seq_t
a(idx_t n)
{
    /* make sure the cache covers [0..n] */
    ensure_cache(n);

    /* bootstrap first two values if not yet done */
    if (cache_filled == 0) {
        cache[0] = 0;
        cache[1] = 1;
        cache_filled = 2;
    }

    /* return cached value if already computed */
    if ((size_t)n < cache_filled)
        return cache[n];

    /* compute all missing terms up to n and cache them */
    for (idx_t i = (idx_t)cache_filled; i <= n; ++i) {
        cache[i] = cache[i-1] * (seq_t)10 + cache[i-2] * (seq_t)7;
    }
    cache_filled = (size_t)n + 1;
    return cache[n];
}
