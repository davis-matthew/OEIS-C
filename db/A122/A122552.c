/*
 * A122552  a(0)=a(1)=a(2)=1, a(n) = a(n-1) + a(n-2) + 2*a(n-3) for n > 2.
 * https://oeis.org/A122552
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + 2*a(n-3) * Initial values: a(0) = a(1)
 *            = a(2) = 1
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_seq_array.h" /* seq_t* alloc_seq_array(size_t) */
#include "../utils/free_seq_array.h"  /* void free_seq_array(seq_t*) */

/* Static cache for memoisation */
static seq_t *cache    = NULL;   /* array of computed values */
static size_t  cap     = 0;      /* allocated capacity (number of elements) */
static idx_t   last    = (idx_t)-1; /* index of highest computed element */

/* Return the n‑th term of OEIS A122552 */
seq_t a(idx_t n)
{
    /* base cases (a(0) = a(1) = a(2) = 1) */
    if (n < 3)
        return (seq_t)1;

    /* grow the cache if necessary */
    size_t needed = (size_t)n + 1;      /* we need indices 0..n inclusive */
    if (needed > cap) {
        seq_t *new_cache = alloc_seq_array(needed);
        if (!new_cache)              /* allocation failed – return 0 */
            return (seq_t)0;

        /* copy previously computed values */
        if (cache) {
            for (size_t i = 0; i < cap; ++i)
                new_cache[i] = cache[i];
            free_seq_array(cache);
        }

        /* initialise base values if this is the first allocation */
        if (!cache) {
            new_cache[0] = new_cache[1] = new_cache[2] = (seq_t)1;
            last = 2;   /* we have computed up to index 2 */
        }

        cache = new_cache;
        cap   = needed;
    }

    /* compute any missing values up to n */
    idx_t start = last + 1;   /* start after the last computed index */
    if (last < 2)            /* safety: we must start at 3 if less than 2 */
        start = (idx_t)3;
    for (idx_t i = start; i <= n; ++i) {
        cache[i] = cache[i-1] + cache[i-2] + (cache[i-3] << 1);
    }
    last = n;

    return cache[n];
}
