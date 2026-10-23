/*
 * A122021  a(n) = a(n-2) - (n-1)*a(n-3), with a(0) = 0, a(1) = 1, a(2) = 2.
 * https://oeis.org/A122021
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n-2) - (n-1)*a(n-3), a(0) = 0, a(1) = 1, a(2) = 2
 * Range:     i8 n <= 10, i16 n <= 16, i32 n <= 27, i64 n <= 45, i128 n <= 77,
 *            big any n
 * Helpers:   alloc_seq, free_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_seq.h"
#include "../utils/free_seq.h"

/* Static cache for memoisation */
static seq_t *cache = NULL;      /* array of computed terms */
static idx_t cache_cap = 0;      /* current capacity of the array */
static idx_t last_filled = 0;    /* highest index computed so far */

/* Grow the cache to hold indices up to n (inclusive). */
static void
ensure_capacity(idx_t n)
{
    if (n + 1 <= cache_cap)
        return;

    idx_t new_cap = (cache_cap == 0 ? 4 : cache_cap * 2);
    while (new_cap <= n)
        new_cap <<= 1;           /* double until large enough */

    seq_t *newcache = alloc_seq(new_cap);
    if (!newcache)         /* allocation failed – leave cache untouched */
        return;

    for (idx_t i = 0; i < cache_cap; ++i)
        newcache[i] = cache[i];

    if (cache)
        free_seq(cache);

    cache      = newcache;
    cache_cap  = new_cap;
}

/* Sequence function ----------------------------------------------------- */
seq_t
a(idx_t n)
{
    if (!cache) {
        /* initialise cache with base cases */
        ensure_capacity(2);                     /* ensures at least 3 slots   */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        last_filled = 2;
    }

    ensure_capacity(n);

    /* compute missing values */
    for (idx_t i = last_filled + 1; i <= n; ++i) {
        /* a(i) = a(i-2) - (i-1)*a(i-3) */
        seq_t mult = (seq_t)(i - 1) * cache[i - 3];
        cache[i] = cache[i - 2] - mult;
    }

    last_filled = (last_filled < n ? n : last_filled);
    return cache[n];
}
