/*
 * A206423  Fibonacci sequence beginning 12, 7.
 * https://oeis.org/A206423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 43, u64 n <= 89, u128 n <= 181,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A206423 (Fibonacci-like with initial terms 12, 7, offset 1).
   Recurrence: a(1)=12, a(2)=7, a(n)=a(n-1)+a(n-2) for n>2. */

#include <stdlib.h>

/* Cached values: cache[i-1] holds a(i).  cache_len is the number of
   terms currently stored.  We allocate lazily and grow the cache
   whenever a larger n is requested. */
static seq_t *cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n)
{
    /* Handle zero and negative indices gracefully – they are outside
       the defined offset but may be requested. */
    if (n == 0) return (seq_t)0;

    /* Ensure the cache contains at least the first two terms. */
    if (cache_len == 0) {
        /* Allocate space for the first two terms. */
        cache_len = 2;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        cache[0] = (seq_t)12;  /* a(1) */
        cache[1] = (seq_t)7;   /* a(2) */
    }

    /* If the requested term is already cached, return it. */
    if (n <= cache_len) {
        return cache[n - 1];
    }

    /* Grow the cache to accommodate term n. */
    seq_t *new_cache = (seq_t *)realloc(cache, n * sizeof(seq_t));
    if (!new_cache) {
        /* Allocation failed – return 0 as a sensible error value. */
        return (seq_t)0;
    }
    cache = new_cache;

    /* Compute missing terms iteratively, using the recurrence. */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        /* a(i) = a(i-1) + a(i-2).  Index into cache as (i-1)-1, etc. */
        cache[i - 1] = cache[i - 2] + cache[i - 3];
    }
    cache_len = n;

    return cache[n - 1];
}
