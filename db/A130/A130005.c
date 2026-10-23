/*
 * A130005  Nonnegative values x of solutions (x, y) to the Diophantine
 *          equation x^2+(x+577)^2 = y^2.
 * https://oeis.org/A130005
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-3) - a(n-6) + 1154 for n > 6, a(1) = 0, a(2) = 35,
 *            a(3) = 1568, a(4) = 1731, a(5) = 1908, a(6) = 10595
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External function required by the build system */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t last_allocated = 0;

    if (n == 0)          /* sequence is defined only for n >= 1 */
        return (seq_t)0;

    /* initialize cache with the first 6 terms, if not already done */
    if (!cache) {
        size_t sz = 7; /* indices 0..6, index 0 unused but kept for simplicity */
        cache = (seq_t *)malloc(sz * sizeof(seq_t));
        if (!cache)        /* allocation failure, return 0 as a fallback */
            return (seq_t)0;
        for (idx_t i = 0; i < sz; ++i)
            cache[i] = (seq_t)0;

        cache[1] = (seq_t)0;
        cache[2] = (seq_t)35;
        cache[3] = (seq_t)1568;
        cache[4] = (seq_t)1731;
        cache[5] = (seq_t)1908;
        cache[6] = (seq_t)10595;
        last_allocated = 6;
    }

    /* return cached value if already computed */
    if (n <= last_allocated)
        return cache[n];

    /* grow cache to fit up to n */
    size_t new_sz = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
    if (!tmp)           /* memory allocation failure */
        return (seq_t)0;
    cache = tmp;

    /* compute new terms using the recurrence */
    for (idx_t i = last_allocated + 1; i <= n; ++i) {
        cache[i] = ((seq_t)6 * cache[i - 3]) - cache[i - 6] + (seq_t)1154;
    }
    last_allocated = n;
    return cache[n];
}
