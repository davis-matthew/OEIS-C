/*
 * A130844  a(n) = 2*a(n-1) + a(n-2) - a(n-3) + a(n-4), with a(1) = 0, a(2) =
 *          3, a(3) = 5 and a(4) = 17.
 * https://oeis.org/A130844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-2) - a(n-3) + a(n-4); a(1)=0, a(2)=3,
 *            a(3)=5, a(4)=17 offset 1 (a(1) is the first term)
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 27, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache   = NULL;   /* array of computed terms (indices 0…cap‑1) */
    static idx_t  cache_sz = 0;     /* allocated size (new_cap)                     */
    static idx_t  upto     = 0;     /* highest index computed so far                */

    /* Undefined for n < 1, return 0 from the call site safely */
    if (n == 0u) return 0u;

    /* initialise cache the first time */
    if (cache == NULL) {
        /* we need room for indices 0..4 at least (index 0 unused) */
        cache_sz = 5;
        cache = (seq_t *)malloc(cache_sz * sizeof(seq_t));
        if (cache == NULL) return 0u;   /* allocation failed – unlikely */
    }

    /* expand cache if necessary (ensure capacity for index n) */
    if ((idx_t)n >= cache_sz) {
        /* grow at least to n+1; double for amortised O(1) growth */
        idx_t new_sz = n + 1;
        if (new_sz < cache_sz * 2) new_sz = cache_sz * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
        if (tmp == NULL) return 0u; /* allocation failed – unlikely */
        cache = tmp;
        cache_sz = new_sz;
    }

    /* initialise the first four terms once */
    if (upto < 1) { cache[1] = 0u; }
    if (upto < 2) { cache[2] = 3u; }
    if (upto < 3) { cache[3] = 5u; }
    if (upto < 4) { cache[4] = 17u; }
    if (upto < 4) { upto = 4; }

    /* already computed? */
    if (n <= upto) return cache[n];

    /* compute missing terms up to n */
    for (idx_t i = upto + 1; i <= n; ++i) {
        cache[i] = ( (seq_t)2 * cache[i - 1] )
                 + cache[i - 2]
                 - cache[i - 3]
                 + cache[i - 4];
    }

    upto = n;
    return cache[n];
}
