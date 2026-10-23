/*
 * A008368  Number of orbits on points that are at n steps from the origin in
 *          the f.c.c. lattice.
 * https://oeis.org/A008368
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A008368: Number of orbits on points that are at n steps
 * from the origin in the f.c.c. lattice.
 *
 * Recurrence (obtained from the rational generating function)
 *   a(0)=1, a(1)=1, a(2)=3, a(3)=4, a(4)=7, a(5)=8,
 *   a(6)=13, a(7)=14
 *   a(n) = a(n-1)+2*a(n-2)-a(n-3)-2*a(n-4)-a(n-5)
 *          +2*a(n-6)+a(n-7)-a(n-8)    for all n ≥ 8
 *
 * The function keeps a memoised cache so that subsequent calls
 * are computed in O(1) time after the first up‑to‑required call.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Local cache for memoisation */
    static seq_t *cache          = NULL;
    static idx_t   cache_len     = 0;   /* number of elements already computed */
    static size_t  cache_cap     = 0;   /* allocated capacity */

    /* Predefined first eight terms */
    static const seq_t init[8] = {1, 1, 3, 4, 7, 8, 13, 14};

    /* Requested number of elements */
    size_t need = (size_t)(n + 1);

    /* Allocate or grow the cache if necessary */
    if (cache == NULL) {
        /* Ensure we can hold at least the first eight terms */
        cache_cap = (need >= 8 ? need : 8);
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) { /* allocation failed – return 0 as a sentinel */
            return (seq_t)0;
        }
    } else if (need > cache_cap) {
        size_t new_cap = (cache_cap * 2 > need) ? cache_cap * 2 : need;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) { /* allocation failed – keep old cache */
            return (seq_t)0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    if (cache_len <= n) {
        /* Fill any missing initial values first */
        for (idx_t i = cache_len; i < 8 && i <= n; ++i) {
            cache[(size_t)i] = init[(size_t)i];
        }
        /* If we still need more terms, use the recurrence */
        for (idx_t i = cache_len > 8 ? cache_len : 8; i <= n; ++i) {
            cache[(size_t)i] =
                cache[(size_t)(i-1)]
                + 2 * cache[(size_t)(i-2)]
                - cache[(size_t)(i-3)]
                - 2 * cache[(size_t)(i-4)]
                - cache[(size_t)(i-5)]
                + 2 * cache[(size_t)(i-6)]
                + cache[(size_t)(i-7)]
                - cache[(size_t)(i-8)];
        }
        cache_len = n + 1;
    }

    return cache[(size_t)n];
}
