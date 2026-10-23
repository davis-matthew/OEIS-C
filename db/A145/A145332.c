/*
 * A145332  Numbers Y such that 129*Y^2+43 is a square.
 * https://oeis.org/A145332
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 9,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=53, a(2)=1786683, a(n+2)=33710*a(n+1)-a(n) */
#include <stddef.h>
/* The following helper ensures that the array has room for
   indices 0 … n (index 0 is unused for convenience). */
#include "../utils/ensure_cache.h"

/* Return the nth term of the sequence.  Indexing starts at 1. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                       /* undefined for zero or negative indices */

    static seq_t *cache = NULL;          /* 0‑based array, cache[0] unused */
    static size_t cache_cap = 0;         /* allocated size (in seq_t elements) */
    static idx_t  max_computed = 0;      /* highest index already filled */

    /* Make sure the array can hold index n */
    ensure_cache(n, &cache, &cache_cap);

    /* Initialise first two terms if we haven't yet */
    if (max_computed < 2) {
        cache[0] = 0;                     /* placeholder for unused index 0 */
        cache[1] = (seq_t)53;
        cache[2] = (seq_t)1786683;
        max_computed = 2;
    }

    /* Compute missing terms up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* a(i) = 33710*a(i-1) - a(i-2) */
        cache[i] = (seq_t)33710 * cache[i - 1] - cache[i - 2];
    }
    max_computed = n;
    return cache[n];
}
