/*
 * A145027  a(n) = a(n-1) + a(n-2) + a(n-3) with a(1) = 2, a(2) = 3, a(3) = 4.
 * https://oeis.org/A145027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3) Initial values: a(1) = 2, a(2) =
 *            3, a(3) = 4
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 36, u64 n <= 73, u128 n <= 146,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This implementation uses memoisation with an expanding static array */
/*  and is valid in C11 as well as C++17 (void* casts are performed). */

#include <stdlib.h>  /* malloc, realloc */

seq_t a(idx_t n)
{
    /* static cache of already computed values */
    static seq_t *cache = (seq_t *)NULL;
    static size_t cap   = 0;          /* allocated array size (in elements) */
    static idx_t max_computed = 0;    /* largest n already stored in cache */

    /* If n is 0, the sequence is undefined; return 0 as a safe default. */
    if (n == 0)
        return (seq_t)0;

    /* Initialise cache if this is the first call. */
    if (cache == (seq_t *)NULL) {
        /* We need space for indices 1..max(3, n).  +1 because storage
           is 1‑based; we ignore index 0 for simplicity. */
        cap = (size_t)((n > 3) ? (n + 1) : 4);
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == (seq_t *)NULL)   /* allocation failed: abort gracefully */
            return (seq_t)0;

        /* set the known initial values */
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)4;
        /* Fill any unused part up to cap with zero (optional) */
        for (size_t i = 4; i < cap; ++i)
            cache[i] = (seq_t)0;

        max_computed = 3;
    }

    /* Grow the cache if the requested n exceeds current capacity. */
    if ((idx_t)(cap) <= n) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == (seq_t *)NULL)   /* reallocation failed: abort gracefully */
            return (seq_t)0;
        cache = tmp;
        /* initialise the newly added slots to zero */
        for (size_t i = cap; i < new_cap; ++i)
            cache[i] = (seq_t)0;
        cap = new_cap;
    }

    /* Compute any missing terms up to n. */
    for (idx_t i = (idx_t)(max_computed) + 1; i <= n; ++i)
        cache[i] = cache[i-1] + cache[i-2] + cache[i-3];

    max_computed = (max_computed > n) ? max_computed : n;
    return cache[n];
}
