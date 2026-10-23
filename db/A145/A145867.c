/*
 * A145867  Number of involutions of length 2n which are invariant under the
 *          reverse-complement map and have no decreasing subsequence of
 *          length 7.
 * https://oeis.org/A145867
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   seq_alloc_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   (n+2)*(n+4)*a(n) = 6*(n^2 + 3*n + 1)*a(n-1)
                    + 4*(n-1)*(n+1)*a(n-2)
                    - 24*(n-2)*(n-1)*a(n-3)
   Initial values: a(0)=1, a(1)=2, a(2)=6. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_alloc_array.h"
#include "../utils/seq_free_array.h"

seq_t a(idx_t n)
{
    /* static memoisation storage */
    static seq_t *cache      = NULL;
    static idx_t cache_cap   = 0;   /* buffer size in elements      */
    static idx_t cache_size  = 0;   /* next free index              */

    /* initialise cache with the first three known terms */
    if (cache == NULL) {
        cache_cap   = 64;                               /* initial capacity          */
        cache       = seq_alloc_array(cache_cap);       /* allocates seq_t array     */
        if (cache == NULL) return (seq_t)0;             /* allocation failed         */
        cache[0]    = (seq_t)1;                         /* a(0) */
        cache[1]    = (seq_t)2;                         /* a(1) */
        cache[2]    = (seq_t)6;                         /* a(2) */
        cache_size  = 3;                                /* next free index           */
    }

    /* already computed ? */
    if (n < cache_size)
        return cache[n];

    /* enlarge cache if necessary */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap << 1 : 64;
        while (new_cap <= n) new_cap <<= 1;

        seq_t *new_cache = seq_alloc_array(new_cap);
        if (new_cache == NULL) return (seq_t)0;         /* allocation failed */

        /* copy existing values */
        for (idx_t i = 0; i < cache_cap; ++i)
            new_cache[i] = cache[i];

        /* free old buffer */
        seq_free_array(cache);

        cache       = new_cache;
        cache_cap   = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t ii  = (seq_t)i;                     /* cast index to seq_t */

        /* coefficients of the recurrence */
        seq_t A   = ii*ii + 3*ii + 1;             /* n^2 + 3n + 1 */
        seq_t B   = (ii-1)*(ii+1);                /* (n-1)(n+1) */
        seq_t C   = (ii-2)*(ii-1);                /* (n-2)(n-1) */

        /* right‑hand side of the recurrence */
        seq_t term1 = 6 * A * cache[i-1];
        seq_t term2 = 4 * B * cache[i-2];
        seq_t term3 = 24 * C * cache[i-3];
        seq_t rhs   = term1 + term2 - term3;

        /* denominator (n+2)(n+4) */
        seq_t denom = (ii + 2) * (ii + 4);

        cache[i] = rhs / denom;   /* exact division */
    }

    cache_size = n + 1;            /* updated next free index */
    return cache[n];
}
