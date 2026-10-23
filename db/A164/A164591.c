/*
 * A164591  a(n) = ((4 + sqrt(18))*(4 + sqrt(8))^n + (4 - sqrt(18))*(4 -
 *          sqrt(8))^n)/8 .
 * https://oeis.org/A164591
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - 8*a(n-2), with a(0)=1, a(1)=7
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of elements currently stored (cache_len == last_index+1) */
static size_t cache_cap = 0;   /* capacity of the allocated array */

/* Expand the cache so that it contains values up to index n. */
static void grow_cache(idx_t n)
{
    if (cache_len > n)          /* already computed up to n */
        return;

    /* Grow capacity if necessary */
    if (cache_cap == 0)
        cache_cap = 16;
    while (cache_cap <= (size_t)n)
        cache_cap <<= 1;

    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));

    /* Initialise the base cases if they are not yet stored */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;          /* a(0) */
        cache_len = 1;
    }
    if (n >= 1 && cache_len == 1) {   /* a(1) only needed when n>=1 */
        cache[1] = (seq_t)7;          /* a(1) */
        cache_len = 2;
    }

    /* Compute any missing terms up to n */
    for (idx_t i = (cache_len > 1 ? cache_len : 2); i <= n; ++i) {
        /* a(i) = 8*a(i-1) - 8*a(i-2) */
        cache[i] = (seq_t)8 * cache[i-1] - (seq_t)8 * cache[i-2];
    }

    cache_len = (size_t)n + 1;   /* now we have entries 0..n inclusive */
}

seq_t a(idx_t n)
{
    grow_cache(n);
    return cache[n];
}
