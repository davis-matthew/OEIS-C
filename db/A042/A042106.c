/*
 * A042106  Numerators of continued fraction convergents to sqrt(578).
 * https://oeis.org/A042106
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(n) = 1154*a(n-2) - a(n-4) a(0)=24, a(1)=577, a(2)=27720,
 *            a(3)=665857
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Return the n-th term of the sequence.  The function keeps a static
   cache so that any n can be queried, in any order, in O(n) time
   for the largest n asked for. */
seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;      /* pointer to cached values */
    static size_t cache_len = 0;     /* number of values stored */
    static size_t cache_cap = 0;     /* allocated capacity */

    size_t idx = (size_t)n;          /* size_t is safe for array index */

    /* Grow the cache if necessary */
    if (idx + 1 > cache_cap) {
        size_t new_cap = idx + 1;
        if (new_cap < 8) new_cap = 8;        /* minimal initial size */
        new_cap *= 2;                       /* double it */
        cache = (seq_t *)realloc(cache,
                                  new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute missing terms up to idx */
    if (idx >= cache_len) {
        for (size_t i = cache_len; i <= idx; ++i) {
            if (i == 0)          cache[i] = 24;
            else if (i == 1)     cache[i] = 577;
            else if (i == 2)     cache[i] = 27720;
            else if (i == 3)     cache[i] = 665857;
            else
                cache[i] = (seq_t)1154 * cache[i-2] - cache[i-4];
        }
        cache_len = idx + 1;
    }

    return cache[idx];
}
