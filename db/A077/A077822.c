/*
 * A077822  Expansion of (1-x)^(-1)/(1-3*x-3*x^2-2*x^3).
 * https://oeis.org/A077822
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - a(n-3) - 2*a(n-4) a(0)=1, a(1)=4, a(2)=16,
 *            a(3)=63
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cap = 0;   /* number of slots allocated */
    static size_t last = 3;  /* highest index computed so far */

    if (cache == NULL) {
        cap = 4;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        /* base values */
        cache[0] = 1;
        cache[1] = 4;
        cache[2] = 16;
        cache[3] = 63;
        last = 3;
    }

    size_t N = (size_t)n;
    if (N <= last) return cache[N];

    /* enlarge cache if necessary */
    if (cap <= N) {
        size_t new_cap = cap ? cap : 1;
        while (new_cap <= N) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp) cache = tmp;
        cap = new_cap;
    }

    /* compute new terms */
    for (size_t i = last + 1; i <= N; ++i) {
        cache[i] = 4 * cache[i-1] - cache[i-3] - 2 * cache[i-4];
    }
    last = N;
    return cache[N];
}
