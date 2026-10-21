/*
 * A077908  Expansion of (1-x)^(-1)/(1+x+x^2-x^3).
 * https://oeis.org/A077908
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -a(n-1) - a(n-2) + a(n-3) + 1, with initial values
 *            a(0)=1, a(1)=0, a(2)=0. This implements A077908: Expansion of
 *            1/(1‑2*x^3+x^4)
 * Range:     i8 n <= 18, i16 n <= 35, i32 n <= 73, i64 n <= 145,
 *            i128 n <= 291, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* allocated capacity */
    static idx_t max_computed = (idx_t)-1; /* highest index computed */

    /* initialise cache once */
    if (max_computed == (idx_t)-1) {
        cache_size = 4;                      /* need indices 0..3 */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return 0;                 /* allocation failure – unlikely in tests */
        cache[0] = 1;                         /* a(0) */
        cache[1] = 0;                         /* a(1) */
        cache[2] = 0;                         /* a(2) */
        /* compute a(3) using the recurrence */
        cache[3] = -cache[2] - cache[1] + cache[0] + 1; /* 2 */
        max_computed = 3;
    }

    /* quick return for already known terms */
    if (n <= max_computed)
        return cache[n];

    /* enlarge cache if necessary */
    if (n >= cache_size) {
        idx_t new_size = cache_size;
        while (new_size <= n)
            new_size <<= 1;   /* double until sufficient */
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        cache_size = new_size;
    }

    /* compute missing terms up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t val = -cache[i-1] - cache[i-2] + cache[i-3] + 1;
        cache[i] = val;
    }
    max_computed = n;
    return cache[n];
}
