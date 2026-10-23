/*
 * A042911  Denominators of continued fraction convergents to sqrt(987).
 * https://oeis.org/A042911
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 754*a(n-4) - a(n-8) for n > 7. Initial values: a(0)=1,
 *            a(1)=2, a(2)=5, a(3)=12, a(4)=749, a(5)=1510, a(6)=3769,
 *            a(7)=9048
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t  cache_cap = 0;   /* allocated size */
    static idx_t  cache_filled = 0;/* next index to compute */

    /* initial values */
    static const seq_t init[8] = {1, 2, 5, 12, 749, 1510, 3769, 9048};

    /* Return directly for the base cases */
    if (n < 8)
        return init[n];

    /* Resize cache if needed */
    if (n >= cache_cap || cache == NULL) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;

        /* Initialize the first 8 terms if this is the first allocation */
        if (cache_filled < 8) {
            for (idx_t i = cache_filled; i < 8; ++i)
                cache[i] = init[i];
            cache_filled = 8;
        }
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_filled; i <= n; ++i) {
        /* a(i) = 754 * a(i-4) - a(i-8)   (both operands fit in seq_t) */
        seq_t val = cache[i - 4] * (seq_t)754 - cache[i - 8];
        cache[i] = val;
    }
    cache_filled = n + 1;

    return cache[n];
}
