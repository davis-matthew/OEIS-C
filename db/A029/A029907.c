/*
 * A029907  a(n+1) = a(n) + a(n-1) + Fibonacci(n), with a(0) = 0 and a(1) = 1.
 * https://oeis.org/A029907
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 41, u64 n <= 86, u128 n <= 176,
 *            big any n
 * Helpers:   fib, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A029907: a(n+1) = a(n) + a(n-1) + Fibonacci(n),
 *  a(0) = 0, a(1) = 1.
 */

#include <stddef.h>
#include <stdint.h>
#include <string.h>          /* for memcpy */
#include "../utils/fib.h"    /* seq_t fib(idx_t n) */
#include "../utils/seq_alloc.h"   /* seq_alloc(size_t)       */
#include "../utils/seq_realloc.h" /* seq_realloc(seq_t*,size_t,size_t) */

/* static cache of computed values */
static seq_t *cache = NULL;  /* pointer to the array */
static size_t cache_size = 0; /* number of terms stored */
static size_t cache_cap  = 0; /* allocated capacity */

/* enlarge the cache so it can hold at least `need` elements */
static void ensure_capacity(size_t need)
{
    if (need <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need)
        new_cap <<= 1;                 /* new_cap *= 2 */

    /* reallocate the array */
    seq_t *new_cache = seq_realloc(cache, cache_cap, new_cap);
    cache = new_cache;
    cache_cap = new_cap;
}

/* public function requested by the build system */
seq_t a(idx_t n)
{
    /* initialise first two values on first call */
    if (cache_size == 0) {
        ensure_capacity(2);
        cache[0] = 0;                  /* a(0) */
        cache[1] = 1;                  /* a(1) */
        cache_size = 2;
    }

    /* compute any missing values up to index n */
    if ((idx_t)n >= cache_size) {
        /* Ensure enough capacity for element (size_t)n */
        ensure_capacity((size_t)n + 1);

        /* recurrence: a(i) = a(i-1) + a(i-2) + Fib(i-1) */
        for (idx_t i = cache_size; i <= n; ++i) {
            cache[i] = cache[i-1] + cache[i-2] + fib(i-1);
        }
        cache_size = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
