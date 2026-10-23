/*
 * A140131  a(n) = a(n-1) + a(n-2) + digsum(a(n-1)) + digsum(a(n-2)), with
 *          a(0)=0 and a(1)=1.
 * https://oeis.org/A140131
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = a(n-1) + a(n-2) + digsum(a(n-1)) + digsum(a(n-2)) initial
 *            values a(0) = 0, a(1) = 1. This implementation memoises all
 *            previous values in a static array. It works for any seq_t type
 *            that supports +, -, *, /, %, and assignment. For seq_t =
 *            arbitrary‑precision integers the addition and modulo...
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 181,
 *            big any n
 * Helpers:   digsum, free_cache_ptr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/digsum.h"
#include "../utils/free_cache_ptr.h"

/* Cached values of the sequence. */
static seq_t *cache = NULL;
static size_t cache_cap = 0; /* capacity of allocated array */
static size_t cache_len = 0; /* number of initialised elements */

/* Allocation helpers that work in C and C++. */
static seq_t *alloc_cache(size_t sz)
{
#if defined(__cplusplus)
    return new seq_t[sz];
#else
    return (seq_t *)malloc(sz * sizeof(seq_t));
#endif
}

/* Grow the cache so that it can hold values up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (cache_len == 0) { /* first initialisation */
        size_t init_cap = (size_t)n + 1;
        if (init_cap < 2) init_cap = 2;
        cache = alloc_cache(init_cap);
        cache_cap = init_cap;
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache_len = 2;   /* elements 0..1 are now valid */
    }

    /* Grow array if necessary */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 2;
        while ((size_t)n + 1 > new_cap)
            new_cap <<= 1;
        seq_t *new_cache = alloc_cache(new_cap);
        for (size_t i = 0; i < cache_len; ++i)
            new_cache[i] = cache[i];
        free_cache_ptr(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute missing values */
    if (cache_len <= (size_t)n) {
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            cache[i] = cache[i-1] + cache[i-2]
                     + digsum(cache[i-1]) + digsum(cache[i-2]);
        }
        cache_len = (size_t)n + 1;
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
