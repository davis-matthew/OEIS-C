/*
 * A197201  Number of n X 2 0..4 arrays with each element x equal to the
 *          number its horizontal and vertical neighbors equal to 4,2,1,0,3
 *          for x=0,1,2,3,4.
 * https://oeis.org/A197201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=5, a(3)=16, a(4)=47, a(5)=154, a(6)=496, a(7)=1601,
 *            a(8)=5183 For n≥9: a(n) = 2*a(n‑1) + 4*a(n‑2) + 2*a(n‑3) +
 *            a(n‑7) + 2*a(n‑8) – (4*a(n‑4) + 8*a(n‑5))
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 38, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static memoization cache */
    static seq_t *cache      = NULL;   /* array indexed by n (0 unused)   */
    static size_t cache_cap  = 0;      /* allocated size (indices 0..cap-1) */
    static idx_t  cache_up   = 0;      /* highest n computed so far       */

    /* Ensure the cache is large enough for index n */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = (size_t)n + 1; /* +1 because 0 is unused */
        /* realloc in a cross‑compatible way */
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        /* zero new entries; safer for arbitrary‑precision types */
        for (size_t i = cache_cap; i < new_cap; ++i)
            cache[i] = 0;
        cache_cap = new_cap;
    }

    /* If value already computed, return it */
    if (n <= cache_up)
        return cache[n];

    /* Initialise first eight terms explicitly (index 0 is unused) */
    static const seq_t init[9] = {0, 1, 5, 16, 47, 154, 496, 1601, 5183};
    if (cache_up < 8) {
        idx_t start = (cache_up + 1 > 1) ? cache_up + 1 : 1;
        for (idx_t i = start; i <= 8 && i <= n; ++i)
            cache[i] = init[(size_t)i];
        cache_up = (cache_up < 8) ? 8 : cache_up;
    }

    /* Compute missing terms using the recurrence */
    for (idx_t i = cache_up + 1; i <= n; ++i) {
        if (i <= 8) continue;          /* first eight are already set   */

        seq_t pos = 2 * cache[i-1] + 4 * cache[i-2] + 2 * cache[i-3]
                    + cache[i-7] + 2 * cache[i-8];
        seq_t neg = 4 * cache[i-4] + 8 * cache[i-5];
        cache[i] = pos - neg;
    }

    cache_up = n;
    return cache[n];
}
