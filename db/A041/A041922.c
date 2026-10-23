/*
 * A041922  Numerators of continued fraction convergents to sqrt(483).
 * https://oeis.org/A041922
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(n) = 44*a(n-2)-a(n-4), with a(0)=21, a(1)=22, a(2)=945,
 *            a(3)=967
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */

/* Static cache for computed values.  The cache will grow
   automatically to accommodate any requested index. */
static seq_t *cache = NULL;
static idx_t   cache_size   = 0;  /* number of elements allocated */
static idx_t   computed_idx = 0;  /* next index to compute */

/* Ensure the cache array is large enough to hold indices up to n (inclusive). */
static void ensure_cache_size(idx_t n)
{
    if (n < cache_size) return;
    /* Allocate at least n+1 elements.  Use a conservative growth strategy. */
    idx_t new_size = n + 1;
    if (new_size < 8) new_size = 8;          /* minimal practical size */
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) abort();                       /* Allocation failed */
    cache = tmp;
    cache_size = new_size;
}

/* Compute values up to the requested index n using the recurrence. */
static void compute_up_to(idx_t n)
{
    /* Initialise base cases if this is the first computation. */
    if (computed_idx == 0) {
        if (cache_size < 4) {
            /* In the unlikely case that we were called with n<3 before
               ensure_cache_size expanded the buffer, resize it now. */
            ensure_cache_size(3);
        }
        cache[0] = (seq_t)21;
        cache[1] = (seq_t)22;
        cache[2] = (seq_t)945;
        cache[3] = (seq_t)967;
        computed_idx = 4;   /* next index to compute */
    }

    /* Generate successive terms until we reach n. */
    while (computed_idx <= n) {
        /* a(k) = 44*a(k-2) - a(k-4) */
        seq_t term = cache[computed_idx - 2] * (seq_t)44 - cache[computed_idx - 4];
        cache[computed_idx] = term;
        ++computed_idx;
    }
}

seq_t a(idx_t n)
{
    if (n <= 3) {            /* Direct lookup for the initial four terms. */
        static const seq_t init[4] = {
            (seq_t)21,
            (seq_t)22,
            (seq_t)945,
            (seq_t)967
        };
        return init[n];
    }

    /* Make sure the cache can hold the desired index. */
    ensure_cache_size(n);

    /* Generate values up to n if necessary. */
    if (computed_idx <= n) compute_up_to(n);

    return cache[n];
}
