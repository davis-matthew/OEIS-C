/*
 * A122015  Expansion of -x *
 *          (21*x^7+65*x^6+77*x^5+39*x^4-17*x^3-38*x^2-11*x-1) / ((x+1) *
 *          (x^3+x^2-1) * (x^4+2*x^3+x^2+x-1)).
 * https://oeis.org/A122015
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less, easy
 * Formula:   Recurrence: a(n) = 3*a(n-2) + 4*a(n-3) + a(n-4) - 4*a(n-5) -
 *            6*a(n-6) - 4*a(n-7) - a(n-8) Initial values (OEIS offset 1):
 *            a(1)=1, a(2)=11, a(3)=41, a(4)=54, a(5)=129, a(6)=256, a(7)=529,
 *            a(8)=1083
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 28, u64 n <= 59, u128 n <= 120,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Initial terms a(1)..a(8) */
    static const seq_t init[9] = {0, 1, 11, 41, 54, 129, 256, 529, 1083};

    /* If n is within the initial block, return it immediately. */
    if (n <= 8) return init[(size_t)n];

    /* Static cache to memoize computed terms. */
    static seq_t *cache = NULL;      /* array of computed terms, indexed by n */
    static size_t cache_cap = 0;     /* allocated size of cache */
    static idx_t last_computed = 0;  /* highest n that has been computed */

    /* Allocate cache and fill initial values on first call. */
    if (cache == NULL) {
        cache_cap = 9;                            /* indices 0..8, index 0 unused */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        for (size_t i = 1; i < 9; ++i) cache[i] = init[i];
        last_computed = 8;
    }

    /* Extend cache if we need values beyond what we have. */
    if (n > last_computed) {
        size_t new_cap = (size_t)n + 1;
        if (new_cap > cache_cap) {
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }
        /* Compute successive terms using the recurrence. */
        for (idx_t cur = last_computed + 1; cur <= n; ++cur) {
            seq_t t1 = 3 * cache[(size_t)(cur - 2)];
            t1 += 4 * cache[(size_t)(cur - 3)];
            t1 +=           cache[(size_t)(cur - 4)];
            seq_t t2 = 4 * cache[(size_t)(cur - 5)];
            t2 += 6 * cache[(size_t)(cur - 6)];
            t2 += 4 * cache[(size_t)(cur - 7)];
            t2 +=           cache[(size_t)(cur - 8)];
            cache[(size_t)cur] = t1 - t2;            /* t1 >= t2 for all valid n */
        }
        last_computed = n;
    }

    return cache[(size_t)n];
}
