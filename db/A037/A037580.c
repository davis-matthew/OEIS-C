/*
 * A037580  Base 8 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,3.
 * https://oeis.org/A037580
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 8*a(n-1) + a(n-2) - 8*a(n-3) with initial terms a(1)=1,
 *            a(2)=11, a(3)=89
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cached sequence values */
static seq_t *cache = NULL;
static idx_t cache_size = 0;

/* Return a(n) for n ≥ 1 */
seq_t a(idx_t n)
{
    /* Out of offset: return 0 (not used) */
    if (n == 0)
        return 0;

    /* If cache not yet initialised, allocate space for the first three terms */
    if (cache_size < 3) {
        cache = (seq_t *)realloc(cache, (size_t)3 * sizeof(seq_t));
        /* initialise first three terms (indices 1,2,3) */
        cache[0] = (seq_t)1;   /* a(1) */
        cache[1] = (seq_t)11;  /* a(2) */
        cache[2] = (seq_t)89;  /* a(3) */
        cache_size = 3;
    }

    /* If the requested term is already cached */
    if (n <= cache_size)
        return cache[n - 1];

    /* Grow the cache to hold up to term n */
    cache = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));

    /* Compute missing terms using the recurrence */
    for (idx_t i = cache_size; i < n; ++i) {
        /* i corresponds to a(i+1) */
        seq_t val = (seq_t)8 * cache[i - 1] + cache[i - 2] - (seq_t)8 * cache[i - 3];
        cache[i] = val;
    }
    cache_size = n;
    return cache[n - 1];
}
