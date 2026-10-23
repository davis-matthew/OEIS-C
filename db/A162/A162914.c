/*
 * A162914  Number of reduced words of length n in Coxeter group on 49
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^3 = I.
 * https://oeis.org/A162914
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(1) = 49 a(2) = 2352 a(3) = 111720 a(n) = 47*a(n-1) +
 *            47*a(n-2) - 1128*a(n-3) for n >= 4
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for all computed terms */
    static seq_t *cache = NULL;
    static idx_t  max_n  = (idx_t)-1;   /* highest index currently stored */
    static size_t cap    = 0;          /* allocated array size */

    /* initialize base values on first call */
    if (cache == NULL) {
        cap = 8; /* initial capacity */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* allocation failure - fallback */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)49;
        cache[2] = (seq_t)2352;
        cache[3] = (seq_t)111720;
        max_n  = 3;
    }

    /* extend cache if needed */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap * 2;
        if (new_cap < (size_t)n + 1)
            new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failure - fallback */
        cache = tmp;
        cap   = new_cap;
    }

    /* compute missing terms up to n */
    if ((idx_t)n > max_n) {
        for (idx_t i = max_n + 1; i <= n; ++i) {
            cache[i] = (seq_t)(47 * cache[i - 1]
                               + 47 * cache[i - 2]
                               - 1128 * cache[i - 3]);
        }
        max_n = n;
    }

    return cache[n];
}
