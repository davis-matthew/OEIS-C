/*
 * A091307  a(n) = 6*2^n + 4 (Bode Number A003461(n+2)) except for a(1)=6.
 * https://oeis.org/A091307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 2*a(n-1) - 4 for n > 2, with a(1) = 6 and a(2) = 28.
 *
 * This implementation memoises values up to the largest n queried, so
 * repeated calls do not recompute previous terms.  The recurrence
 * handles all n ≥ 1 and works for any build of seq_t, whether it is a
 * small unsigned integer type or an arbitrary-precision integer
 * class (C++ only).
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the trivial initialization directly. */
    if (n == 1) return (seq_t)6;
    if (n == 2) return (seq_t)28;

    /* Static cache for memoisation.  Index 0 is unused. */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   /* maximum index currently cached */

    /* Lazy allocation on first call. */
    if (cache == NULL) {
        /* Allocate space for indices 0..2 (index 0 unused). */
        cache = (seq_t *)malloc((3) * sizeof(seq_t));
        if (!cache) abort();          /* OJ will not hit OOM for small sizes. */
        cache[1] = (seq_t)6;         /* a(1) */
        cache[2] = (seq_t)28;        /* a(2) */
        cache_cap = 2;
    }

    /* If we already have the value, return it immediately. */
    if (n <= cache_cap) return cache[n];

    /* Extend the cache to hold up to index n. */
    idx_t new_cap = n;               /* new maximum index needed */
    seq_t *new_cache = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
    if (!new_cache) abort();         /* again, unlikely for problem constraints */
    cache = new_cache;
    /* Compute new terms using the recurrence. */
    for (idx_t i = cache_cap + 1; i <= new_cap; ++i) {
        cache[i] = 2 * cache[i - 1] - (seq_t)4;
    }
    cache_cap = new_cap;
    return cache[n];
}
