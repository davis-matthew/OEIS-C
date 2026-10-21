/*
 * A240847  a(n) = 2*a(n-1) + a(n-2) - 2*a(n-3) - a(n-4) for n>3,
 *          a(0)=a(1)=a(3)=0, a(2)=1.
 * https://oeis.org/A240847
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-2) - 2*a(n-3) - a(n-4) Initial values:
 *            a(0)=0, a(1)=0, a(2)=1, a(3)=0
 * Range:     i8 n <= 12, i16 n <= 22, i32 n <= 43, i64 n <= 87, i128 n <= 178,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t last  = 3;               /* highest index already computed */

    /* initialise with the four known terms */
    if (!cache) {
        cache = (seq_t *)malloc(4 * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        cache[0] = 0;
        cache[1] = 0;
        cache[2] = 1;
        cache[3] = 0;
        cap   = 4;
        last  = 3;
    }

    /* fast return for pre‑computed indices */
    if (n <= (idx_t)last) return cache[n];

    /* enlarge cache if the requested index is beyond current capacity */
    if ((size_t)n >= cap) {
        size_t newcap = cap * 2;
        if (newcap <= (size_t)n) newcap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cap   = newcap;
    }

    /* compute missing terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        cache[i] = 2 * cache[i-1] + cache[i-2] - 2 * cache[i-3] - cache[i-4];
    }
    last = n;
    return cache[n];
}
