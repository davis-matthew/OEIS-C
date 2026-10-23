/*
 * A126116  a(n) = a(n-1) + a(n-3) + a(n-4), with a(0)=a(1)=a(2)=a(3)=1.
 * https://oeis.org/A126116
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-3) + a(n-4), a(0)=a(1)=a(2)=a(3)=1
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 47, u64 n <= 94, u128 n <= 186,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS A126116.  The sequence is memoized in a
   static array so that calls in any order are fast and repeated calls to the
   same or lower index cost nothing.  The implementation works for any
   integer type seq_t supplied by the build. */
#include <stdlib.h>

static seq_t *cache      = NULL;   /* memoization array */
static size_t cache_len = 0;       /* number of entries currently stored */

seq_t a(idx_t n)
{
    /* Ensure the argument can be represented as a size_t index for the array. */
    size_t idx = (size_t)n;

    /* If we already have the value, return it immediately. */
    if (idx < cache_len)
        return cache[idx];

    /* Grow the cache to accommodate the new index.  Use a size that is just
       large enough (no extra over‑allocation).  Since n may be large, use
       unsigned 64‑bit arithmetic converting to size_t only when safe. */
    size_t new_len = idx + 1;
    if (new_len < 4)              /* ensure we always store the first four terms */
        new_len = 4;

    /* Reallocate the cache array.  Using realloc allows us to keep the
       previously computed values. */
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (tmp == NULL) {            /* allocation failure – abort gracefully */
        exit(EXIT_FAILURE);
    }
    cache = tmp;

    /* Compute all new entries sequentially.  This guarantees that when we
       encounter an index i ≥ 4, the indices i-1, i-3, and i-4 have already
       been computed.  For indices 0..3 we simply set the initial values. */
    for (size_t i = cache_len; i < new_len; ++i) {
        if (i == 0 || i == 1 || i == 2 || i == 3) {
            cache[i] = 1;
        } else {
            cache[i] = cache[i-1] + cache[i-3] + cache[i-4];
        }
    }

    /* Update the stored length and return the requested value. */
    cache_len = new_len;
    return cache[idx];
}
