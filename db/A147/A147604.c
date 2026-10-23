/*
 * A147604  Expansion of g.f.: (1 + x^2 - x^3)/(1 - x - x^2 + x^3 - x^5).
 * https://oeis.org/A147604
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 30, u32 n <= 61, u64 n <= 121,
 *            u128 n <= 242, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=1, a(2)=3, a(3)=2, a(4)=4, a(5)=4,
   a(n)=a(n‑1)+a(n‑2)-a(n‑3)+a(n‑5)  for n ≥ 6 */
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;        /* holds a(0) .. a(cap-1) */
    static size_t cap = 0;             /* capacity of cache */

    /* initial population of the cache with the first 6 terms */
    if (cap == 0) {
        /* allocate space for at least 6 entries (indices 0..5) */
        cap = 6;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0; /* abort on OOM */
        cache[0] = 1;
        cache[1] = 1;
        cache[2] = 3;
        cache[3] = 2;
        cache[4] = 4;
        cache[5] = 4;
    }

    /* if the requested index is already computed, return it */
    if ((idx_t)n < (idx_t)cap)
        return cache[(size_t)n];

    /* grow the cache to at least n+1 entries */
    size_t new_cap = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) return 0;  /* abort on OOM */
    cache = tmp;  /* update pointer */
    /* compute new terms from the current capacity up to n */
    for (size_t i = cap; i <= (size_t)n; ++i) {
        seq_t term = 0;
        if (i >= 1) term += cache[i - 1];
        if (i >= 2) term += cache[i - 2];
        if (i >= 3) term -= cache[i - 3];
        if (i >= 5) term += cache[i - 5];
        cache[i] = term;
    }
    cap = new_cap;
    return cache[(size_t)n];
}
