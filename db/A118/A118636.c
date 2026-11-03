/*
 * A118636  Start with 1 and repeatedly reverse the digits and add 40 to get
 *          the next term.
 * https://oeis.org/A118636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 118, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 61 terms through n = 982 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1; a(n) = rev_decimal(a(n-1)) + 40 for n > 1 */

#include <stddef.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Reverse the decimal digits of a seq_t value. */
static seq_t rev_decimal(seq_t x)
{
    seq_t r = 0;
    while (x > 0) {
        seq_t d = x % 10;
        r = r * 10 + d;
        x = x / 10;
    }
    return r;
}

/* allocate an array of seq_t of given length (may be 0) */
static seq_t *seq_alloc_array(size_t n)
{
    return mem_new(seq_t, n);
}

/* C allocation helpers */
static seq_t* seq_alloc_new_array(size_t n)   { return mem_new(seq_t, n); }

/* Memoisation cache, indices 0..cache_max (0 is unused) */
static seq_t *cache      = NULL;
static idx_t cache_max   = 0;   /* highest index that has been computed */

/* Grow the cache to contain terms up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (n <= cache_max)          /* already computed */
        return;

    /* Allocate a new array of size n+1 (indices 0..n) */
    seq_t *new_cache = seq_alloc_array(n + 1);

    /* If we already had a cache, copy its contents and free it */
    if (cache != NULL) {
        mem_copy(new_cache, cache, (cache_max + 1) * sizeof(seq_t));
        mem_free(cache);
    }

    /* Initialise the cache array */
    if (cache_max == 0 && n >= 1) {            /* first call */
        new_cache[1] = (seq_t)1;
        cache_max   = 1;
    }

    /* Compute any missing terms */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t rev = rev_decimal(new_cache[i - 1]);  /* reverse decimal digits */
        new_cache[i] = rev + (seq_t)40;            /* add 40 */
    }

    cache_max = n;
    cache    = new_cache;
}

/* Public entry point for the sequence */
seq_t a(idx_t n)
{
    if (n == 0)  /* undefined by OEIS, return zero for safety */
        return (seq_t)0;

    ensure_cache(n);
    return cache[n];
}
