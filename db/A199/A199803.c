/*
 * A199803  G.f.: 1/(1 + x - x^2 - x^3 + x^4).
 * https://oeis.org/A199803
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -a(n-1) + a(n-2) + a(n-3) - a(n-4) for n > 3, with
 *            a(0)=1, a(1)=-1, a(2)=2, a(3)=-2
 * Range:     i8 n <= 21, i16 n <= 38, i32 n <= 80, i64 n <= 161,
 *            i128 n <= 327, big any n
 * Validated: standard, pass, 74 terms through n = 7610 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, realloc, free */
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* Static cache of computed values */
    static seq_t *cache = NULL;
    static size_t cap   = 0;          /* allocated cache size */
    static idx_t  max_n = (idx_t)-1;  /* highest index computed */

    /* Initialise the cache on first call */
    if (max_n == (idx_t)-1) {
        /* Pre‑allocate space for the first four terms */
        cap   = 4;
        cache = mem_new(seq_t, cap);
        if (!cache) return (seq_t)0;   /* allocation failure – return 0 */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)-1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)-2;
        max_n   = 3;
    }

    /* If the requested index is already cached, return it */
    if (n <= max_n) return cache[n];

    /* Grow the cache if necessary */
    if ((size_t)n >= cap) {
        size_t new_cap = cap;
        while ((size_t)n >= new_cap) {
            /* Double the size until it can hold index n */
            new_cap <<= 1;
        }
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) return (seq_t)0;   /* allocation failure – return 0 */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute the missing terms up to n */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        seq_t val = -cache[i-1] + cache[i-2] + cache[i-3] - cache[i-4];
        cache[i] = val;
    }
    max_n = n;
    return cache[n];
}
