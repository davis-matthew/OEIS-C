/*
 * A083068  7th row of number array A083064.
 * https://oeis.org/A083068
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1)-1, a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>              /* for size_t */
#include "../utils/seq_realloc.h"

/* Cached values of the sequence */
static seq_t *seq_cache = NULL;     /* array holding computed terms   */
static size_t seq_cache_cap = 0;    /* allocated capacity of the cache */
static idx_t  seq_cached_upto = 0;  /* highest index that is valid in seq_cache */

/* Ensure that seq_cache contains terms up to and including index n */
static void seq_ensure_cache(idx_t n)
{
    /* Allocate or grow the cache if necessary */
    if (seq_cache == NULL) {
        size_t cap = 1;
        while (cap <= (size_t)n) cap <<= 1;
        seq_cache = seq_realloc(NULL, 0, cap);
        seq_cache_cap = cap;
        seq_cache[0] = (seq_t)1;        /* a(0) = 1 */
        seq_cached_upto = 0;
    } else if ((size_t)n >= seq_cache_cap) {
        size_t new_cap = seq_cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_cache = seq_realloc(seq_cache, seq_cache_cap, new_cap);
        seq_cache_cap = new_cap;
    }

    /* Compute missing terms using the recurrence a(i) = 8*a(i-1)-1 */
    while (seq_cached_upto < n) {
        seq_t prev = seq_cache[seq_cached_upto];
        seq_t next = prev * (seq_t)8 - (seq_t)1;
        seq_cache[++seq_cached_upto] = next;
    }
}

/* External function that returns the n-th term of the sequence */
seq_t a(idx_t n)
{
    seq_ensure_cache(n);
    return seq_cache[(size_t)n];
}
