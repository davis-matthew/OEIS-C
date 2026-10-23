/*
 * A140787  Expansion of 1 / ( (1+x)*(2*x+1)*(-1+2*x)^2 ).
 * https://oeis.org/A140787
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 59, u128 n <= 122,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) + 6*a(n-2) - 4*a(n-3) - 8*a(n-4),
   a(0) = 1, a(1) = 1, a(2) = 7, a(3) = 9.  This follows from the
   generating function 1/((1+x)*(2*x+1)*(-1+2*x)^2). */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation across calls. */
    static seq_t *cache = NULL;   /* dynamically grown array */
    static idx_t cache_len = 0;   /* current number of valid entries */

    /* Ensure base values are initialised. */
    if (cache_len == 0) {
        /* Allocate space for first four terms. */
        cache = (seq_t *)malloc(4 * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failed */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)7;
        cache[3] = (seq_t)9;
        cache_len = 4;
    }

    /* Grow cache if necessary and compute missing terms. */
    if (n >= cache_len) {
        /* Determine new capacity (double until big enough). */
        idx_t new_cap = cache_len ? cache_len : 4;
        while (new_cap <= n)
            new_cap <<= 1;

        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;      /* allocation failed */
        cache = new_cache;
        /* Compute terms from cache_len to n (inclusive). */
        for (idx_t i = cache_len; i <= n; ++i) {
            /* a(i) = a(i-1) + 6*a(i-2) - 4*a(i-3) - 8*a(i-4) */
            seq_t term = cache[i - 1]
                       + (seq_t)6 * cache[i - 2]
                       - (seq_t)4 * cache[i - 3]
                       - (seq_t)8 * cache[i - 4];
            cache[i] = term;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
