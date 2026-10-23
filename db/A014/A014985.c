/*
 * A014985  a(n) = (1 - (-4)^n)/5.
 * https://oeis.org/A014985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (1 - (-4)^n)/5
 * Range:     i8 n <= 4, i16 n <= 8, i32 n <= 16, i64 n <= 32, i128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Memoization cache.  cache[1] corresponds to a(1).  */
static seq_t *cache     = NULL;
static idx_t cache_cap  = 0; /* number of elements allocated */
static idx_t cache_max  = 0; /* largest n for which cache[n] is valid */

/* Ensure cache has space and is initialized up to (and including) n. */
static void ensure_cache(idx_t n)
{
    /* Grow allocation if necessary. */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 8;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* First‑time initialisation. */
    if (cache_max == 0) {
        cache[1] = (seq_t)1;
        /* cache[2] is also valid after the allocation above. */
        cache[2] = (seq_t)-3;
        cache_max = 2;
    }

    /* Compute missing terms up to n. */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        cache[i] = (seq_t)(-3) * cache[i-1] + (seq_t)4 * cache[i-2];
    }
    cache_max = n;
}

/* Return the n-th term of the sequence (offset 1).  Valid for n >= 1. */
seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;   /* offset starts at 1; behaviour for n<1 is undefined */
    }
    ensure_cache(n);
    return cache[n];
}
