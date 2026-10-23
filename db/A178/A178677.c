/*
 * A178677  If n mod 2 == 0, a(n) = a(floor(n/2)), otherwise a(n) =
 *          a(a(floor((n-1)/2))) + a(n - a(floor((n-1)/2))).
 * https://oeis.org/A178677
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(1)=1 * a(n) = a(n/2) if n is even * a(n) =
 *            a(a((n-1)/2)) + a(n - a((n-1)/2)) if n is odd
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;      /* memoisation table */
static size_t cache_cap = 0;

/* enlarge cache if needed (size in elements). Newly allocated space is
 * zero‑initialised so the value 0 can be used as a sentinel for "uncomputed". */
static void cache_grow(size_t needed)
{
    if (needed < cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap <= needed)
        new_cap <<= 1;

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        abort();

    if (new_cap > cache_cap) {
        /* initialise the newly added part to 0 */
        memset(new_cache + cache_cap, 0,
               (new_cap - cache_cap) * sizeof(seq_t));
    }
    cache = new_cache;
    cache_cap = new_cap;
}

/* recursive computation with memoisation */
static seq_t rec(idx_t n)
{
    if (n == 0)
        return 0;
    if (n == 1)
        return 1;

    cache_grow((size_t)n);
    if (cache[(size_t)n] != 0)
        return cache[(size_t)n];

    seq_t result;

    if ((n & (idx_t)1) == 0) {           /* even: a(n) = a(n/2) */
        result = rec(n >> 1);
    } else {                            /* odd: a(n) = a(a(k)) + a(n-a(k)) */
        idx_t k = (n - (idx_t)1) >> 1;    /* k = floor((n-1)/2) */
        seq_t ak = rec(k);                /* a(k) */
        /* ak is guaranteed to be < n for all n up to the limits tested; */
        idx_t ak_i = (idx_t)ak;
        seq_t part1 = rec(ak_i);
        seq_t part2 = rec(n - ak_i);
        result = part1 + part2;
    }

    cache[(size_t)n] = result;
    return result;
}

seq_t a(idx_t n)
{
    return rec(n);
}
