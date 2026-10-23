/*
 * A014253  a(n) = b(n)^2, where b(n) = b(n-1)^2 + b(n-2)^2 (A000283).
 * https://oeis.org/A014253
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n+2) = (a(n+1) + a(n))^2, a(0) = 0, a(1) = 1
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Static cache to allow arbitrary order of calls and reuse of values */
static seq_t *cache       = NULL;  /* array of computed a(n) */
static size_t cache_cap   = 0;     /* allocated capacity of cache   */
static size_t cache_size  = 0;     /* number of initialized entries */

/* Ensure the cache can hold at least 'needed' elements */
static void
ensure_capacity(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
    while (new_cap <= needed) new_cap <<= 1;
    seq_t *newp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!newp) {
        /* Allocation failed – abort.  In contest settings this is unlikely. */
        abort();
    }
    cache = newp;
    cache_cap = new_cap;
}

/* Return the n-th term of OEIS sequence A014253. */
seq_t
a(idx_t n)
{
    /* Guarantee that cache[0] and cache[1] exist and are initialized. */
    if (cache_size == 0) {
        ensure_capacity(3);          /* need indices 0,1,2 at minimum */
        cache[0] = 0;
        cache[1] = 1;
        cache_size = 2;              /* elements 0 and 1 are set */
    }

    /* If the requested index is not yet computed, extend the cache. */
    if ((size_t)n >= cache_size) {
        ensure_capacity((size_t)n + 1);
        for (size_t i = cache_size; i <= (size_t)n; ++i) {
            seq_t sum = cache[i - 1] + cache[i - 2];
            cache[i]  = sum * sum;   /* compute a(i) = (a(i-1)+a(i-2))^2 */
        }
        cache_size = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
