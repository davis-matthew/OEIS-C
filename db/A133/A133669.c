/*
 * A133669  a(n) = a(n-1) - 49*a(n-2), a(0)=1, a(1)=7.
 * https://oeis.org/A133669
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = a(n-1) - 49*a(n-2), a(0) = 1, a(1) = 7
 * Range:     i8 n <= 2, i16 n <= 5, i32 n <= 10, i64 n <= 20, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;     /* dynamic cache of computed terms */
static idx_t  cache_size = 0;   /* number of valid entries in cache */

/* Compute the n-th term of A133669.  The function is thread‑invariant
   but not reentrant; it assumes calls from a single thread.  It keeps
   the results of past evaluations in a static array so that any
   later evaluation of an arbitrary n can be answered in O(n) time
   and memory. */
seq_t a(idx_t n)
{
    /* First call – set up the cache with at least the first two terms. */
    if (cache_size == 0) {
        /* We need room for at least a(0) and a(1).  For large n we
           allocate enough space to avoid repeated reallocations. */
        idx_t init_size = (n >= 2 ? n + 1 : 2);
        cache = (seq_t *)malloc(init_size * sizeof(seq_t));
        if (!cache) abort();                     /* out of memory */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)7;
        cache_size = 2;

        /* If the initial request requires more terms, compute them. */
        for (idx_t i = 2; i < init_size; ++i) {
            cache[i] = cache[i-1] - (seq_t)49 * cache[i-2];
        }
        cache_size = init_size;
        if (n < cache_size) return cache[n];
    }

    /* If the requested index is beyond what we have, grow the cache. */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        /* Avoid unnecessary reallocs when the new_size is small. */
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!cache) abort();                     /* out of memory */

        /* Fill in the missing terms using the recurrence. */
        for (idx_t i = cache_size; i < new_size; ++i) {
            cache[i] = cache[i-1] - (seq_t)49 * cache[i-2];
        }
        cache_size = new_size;
    }

    return cache[n];
}
