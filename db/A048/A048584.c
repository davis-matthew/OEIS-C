/*
 * A048584  Pisot sequence L(5,7).
 * https://oeis.org/A048584
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-3), initial values: a(0)=5, a(1)=7,
 *            a(2)=10
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 43, u64 n <= 89, u128 n <= 182,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;    /* cache of computed terms */
static size_t cap   = 0;       /* allocated capacity of cache */
static idx_t  last  = 0;       /* highest index computed (-1 if none) */

#ifdef __cplusplus
#include <algorithm>
static void ensure_capacity(size_t new_cap)
{
    if (new_cap <= cap)
        return;
    seq_t *new_cache = new seq_t[new_cap];
    if (cache) {
        for (size_t i = 0; i < cap; ++i)
            new_cache[i] = cache[i];
        delete[] cache;
    }
    cache = new_cache;
    cap   = new_cap;
}
#else
static void ensure_capacity(size_t new_cap)
{
    if (new_cap <= cap)
        return;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) {
        /* Allocation failed – abort */
        abort();
    }
    cache = new_cache;
    cap   = new_cap;
}
#endif

seq_t a(idx_t n)
{
    /* First call: initialise cache with first few terms */
    if (!cache) {
        ensure_capacity(4);          /* minimal initial size */
        cache[0] = (seq_t)5;
        cache[1] = (seq_t)7;
        cache[2] = (seq_t)10;
        last    = 2;
    }

    /* If requested term already computed, return it */
    if (n <= last)
        return cache[(size_t)n];

    /* Ensure cache big enough for n */
    if ((size_t)(n + 1) > cap)
        ensure_capacity((size_t)(n + 1));

    /* Compute missing terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        /* 2*a(i-1) - a(i-3) ; addition first to avoid overflow */
        seq_t two_prev = cache[(size_t)(i - 1)] + cache[(size_t)(i - 1)];
        cache[(size_t)i] = two_prev - cache[(size_t)(i - 3)];
    }
    last = n;
    return cache[(size_t)n];
}
