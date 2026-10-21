/*
 * A084338  a(1) = 1, a(2) = 2, a(3) = 3, a(n+3) = a(n) + a(n+1).
 * https://oeis.org/A084338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-3) + a(n-2), a(1)=1, a(2)=2, a(3)=3
 * Range:     u8 n <= 19, u16 n <= 38, u32 n <= 78, u64 n <= 157,
 *            u128 n <= 315, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

static seq_t *cache = NULL;      /* 1‑based index, cache[0] unused */
static idx_t cache_size = 0;     /* highest computed index */
static idx_t cache_capacity = 0; /* allocated capacity (1‑based) */

/* Ensure that cache has room for indices up to needed (inclusive) */
static void ensure_capacity(idx_t needed)
{
    if (needed <= cache_capacity)
        return;
    idx_t new_cap = cache_capacity ? cache_capacity : 4;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
    if (!new_cache) abort(); /* out of memory */
    cache = new_cache;
    cache_capacity = new_cap;
}

/* Initialize the cache with the first three values */
static void init_cache(void)
{
    ensure_capacity(4);               /* allocate space for indices 0..4 */
    cache[0] = (seq_t)0;              /* unused */
    cache[1] = (seq_t)1;              /* a(1) */
    cache[2] = (seq_t)2;              /* a(2) */
    cache[3] = (seq_t)3;              /* a(3) */
    cache_size = 3;
}

seq_t a(idx_t n)
{
    if (n < 1) /* undefined for nonpositive indices */
        return (seq_t)0;
    if (!cache)
        init_cache();

    if (n <= cache_size)
        return cache[n];

    /* grow cache to hold up to n */
    ensure_capacity(n);

    /* compute missing terms */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        /* recurrence: a(i) = a(i-3) + a(i-2) */
        cache[i] = cache[i - 3] + cache[i - 2];
    }
    cache_size = n;
    return cache[n];
}
