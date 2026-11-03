/*
 * A021941  Decimal expansion of 1/937.
 * https://oeis.org/A021941
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = the n-th decimal digit of 1/937, starting with n=0 after
 *            the point
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Validated: partial, pass, 99 terms through n = 98 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *cache = NULL;          /* cached digits */
static size_t cache_size = 0;        /* allocated size of cache */
static idx_t  cached_last = 0;       /* last index stored in cache (>=0) */
static seq_t   last_rem = 0;         /* remainder after computing digit cached_last */

/* ensure cache is initialized and has capacity for index n */
static void ensure_cache_cap(idx_t n)
{
    if (cache == NULL) {
        /* first time: allocate space for first digit */
        cache_size = 1;
        cache = mem_new(seq_t, cache_size);
        if (cache == NULL) return; /* allocation failure – silently fail */

        /* initialise the first digit (n = 0).  r = 1 initially. */
        seq_t r = 1;                 /* remainder before first digit */
        const seq_t D = (seq_t)937;  /* divisor */
        seq_t digit = (seq_t)10 * r / D;
        cache[0] = digit;
        cached_last = 0;
        last_rem   = (seq_t)10 * r % D;
    }

    /* Grow the cache if necessary */
    while (cache_size <= (size_t)n) {
        size_t new_size = cache_size * 2;
        if (new_size == 0) new_size = 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_size);
        if (tmp == NULL) return;  /* allocation failure – silently fail */
        cache = tmp;
        cache_size = new_size;
    }
}

seq_t a(idx_t n)
{
    /* base case handled in ensure_cache_cap */
    ensure_cache_cap(n);
    if (n <= cached_last) return cache[n];

    const seq_t D = (seq_t)937;  /* divisor */

    /* generate missing digits up to n */
    for (idx_t i = cached_last + 1; i <= n; ++i) {
        seq_t digit = (seq_t)10 * last_rem / D;
        cache[i] = digit;
        last_rem = (seq_t)10 * last_rem % D;
    }
    cached_last = n;
    return cache[n];
}
