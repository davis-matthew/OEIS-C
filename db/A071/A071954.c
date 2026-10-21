/*
 * A071954  a(n) = 4*a(n-1) - a(n-2) - 4, with a(0) = 2, a(1) = 4.
 * https://oeis.org/A071954
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nice, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 34, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 4*a(n-1) - a(n-2) - 4,  a(0)=2, a(1)=4 */

#include <stddef.h>
#include <stdlib.h>

/* Memoisation cache – shared between calls */
static seq_t *seq_cache  = NULL;
static size_t seq_cache_len = 0;

seq_t a(idx_t n)
{
    size_t idx = (size_t)n;

    /* Initialise the cache with the first two terms */
    if (seq_cache == NULL) {
        seq_cache_len = 2;
        seq_cache = (seq_t *)malloc(seq_cache_len * sizeof(seq_t));
        if (!seq_cache) return (seq_t)0;          /* allocation failure */
        seq_cache[0] = (seq_t)2;                 /* a(0) */
        seq_cache[1] = (seq_t)4;                 /* a(1) */
    }

    /* Return a cached value if it already exists */
    if (idx < seq_cache_len)
        return seq_cache[idx];

    /* Grow the cache to contain the requested index */
    size_t old_len = seq_cache_len;
    seq_cache_len = idx + 1;
    seq_t *new_cache = (seq_t *)realloc(seq_cache, seq_cache_len * sizeof(seq_t));
    if (!new_cache) return (seq_t)0;              /* allocation failure */
    seq_cache = new_cache;

    /* Compute missing terms using the recurrence */
    for (size_t i = old_len; i < seq_cache_len; ++i) {
        seq_t val = seq_cache[i-1] * 4;           /* 4 * a(i-1) */
        val -= seq_cache[i-2];                    /* - a(i-2) */
        val -= 4;                                 /* - 4 */
        seq_cache[i] = val;
    }

    return seq_cache[idx];
}
