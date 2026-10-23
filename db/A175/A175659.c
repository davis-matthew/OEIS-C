/*
 * A175659  Eight bishops and one elephant on a 3 X 3 chessboard: a(n)=
 *          (3^(n+1)-Jacobsthal(n+1))-(3^n-Jacobsthal(n)), with
 *          Jacobsthal=A001045.
 * https://oeis.org/A175659
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*a(n-1) - a(n-2) - 6*a(n-3) Initial values: a(0) = 1,
 *            a(1) = 6, a(2) = 16 Recurrence holds for all n >= 3
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoised values */
    static seq_t *cache   = NULL;   /* array of calculated a(k) */
    static size_t  cap    = 0;      /* allocated size of cache      */
    static idx_t   last   = (idx_t)-1; /* largest index computed so far */

    /* Helper to initialise cache with the first three terms */
    if (cache == NULL) {
        cap = 3;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return 0;                 /* allocation failed */
        cache[0] = (seq_t)1;                         /* a(0) */
        cache[1] = (seq_t)6;                         /* a(1) */
        cache[2] = (seq_t)16;                        /* a(2) */
        last = 2;
    }

    /* If the requested index is already in cache, return it */
    if (n <= last) {
        return cache[(size_t)n];
    }

    /* Ensure the cache is large enough to hold up to index n */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;                 /* allocation failed */
        cache = tmp;
        cap = new_cap;
    }

    /* Compute new values up to n, using recurrence */
    for (idx_t i = last + 1; i <= n; ++i) {
        idx_t a1 = i - 1;
        idx_t a2 = i - 2;
        idx_t a3 = i - 3;
        cache[(size_t)i] = 4 * cache[(size_t)a1]
                          - cache[(size_t)a2]
                          - 6 * cache[(size_t)a3];
    }
    last = n;

    return cache[(size_t)n];
}
