/*
 * A203791  Number of (n+1) X 4 0..3 arrays with every 2 X 2 subblock having
 *          equal diagonal elements or equal antidiagonal elements.
 * https://oeis.org/A203791
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 31*a(n-1) - 117*a(n-2) + 72*a(n-3) Initial values (a(1),
 *            a(2), a(3)) are hard‑coded
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 11, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, exit */

seq_t a(idx_t n)
{
    /* Cache for memoisation – works independently of the call order. */
    static seq_t *cache          = NULL;  /* cache[0] unused, cache[1..] are terms */
    static idx_t cache_capacity  = 0;
    static idx_t last_computed   = 0;

    if (n == 0)          /* outside the offset – behaviour not required */
        return 0;

    /* Initialise the cache on first call. */
    if (!cache) {
        cache_capacity = 4;              /* need indices 0..3 at least */
        cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
        if (!cache) exit(1);
        cache[0] = 0;                    /* unused placeholder */
        cache[1] = 5488;                 /* a(1) */
        cache[2] = 142192;               /* a(2) */
        cache[3] = 3784288;              /* a(3) */
        last_computed = 3;
    }

    /* Grow the cache if the requested index is out of range. */
    if (n >= cache_capacity) {
        idx_t new_cap = n + 1;
        if (new_cap < 4) new_cap = 4;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) exit(1);
        cache = new_cache;
        cache_capacity = new_cap;
    }

    /* Return the term if it has already been computed. */
    if (n <= last_computed)
        return cache[n];

    /* Compute missing terms up to n using the recurrence. */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t term = (seq_t)31 * cache[i - 1]
                   - (seq_t)117 * cache[i - 2]
                   + (seq_t)72  * cache[i - 3];
        cache[i] = term;
    }
    last_computed = n;
    return cache[n];
}
