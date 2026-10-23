/*
 * A192651  Coefficient of x^2 in the reduction of the n-th Fibonacci
 *          polynomial by x^3->x^2+x+1. See Comments.
 * https://oeis.org/A192651
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+4*a(n-2)-a(n-3)-4*a(n-4)+a(n-5)+a(n-6) Initial
 *            values: a(1)=0, a(2)=0, a(3)=1, a(4)=1, a(5)=5, a(6)=8
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 30, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle trivial cases directly. */
    if (n == 1 || n == 2) return 0;
    if (n == 3 || n == 4) return 1;
    if (n == 5)          return 5;
    if (n == 6)          return 8;

    /* Static cache for memoisation. */
    static seq_t *cache = NULL;
    static size_t cap = 0;          /* capacity of the cache array */
    static size_t last = 0;         /* highest index that has been computed */

    /* Grow the cache if necessary. */
    if ((size_t)(n + 1) > cap) {
        size_t new_cap = (cap > 0) ? cap : 8;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) return 0;  /* allocation failure – return 0 as fallback */

        if (cache) {
            for (size_t i = 1; i <= last && i < cap; ++i)
                new_cache[i] = cache[i];
            free(cache);
        }
        cache = new_cache;
        cap   = new_cap;

        /* Initialise the first six values. */
        for (idx_t i = 1; i <= 6 && i < cap; ++i) {
            switch (i) {
                case 1: case 2: cache[i] = 0; break;
                case 3: case 4: cache[i] = 1; break;
                case 5:          cache[i] = 5; break;
                case 6:          cache[i] = 8; break;
            }
        }
    }

    /* Compute any missing values up to n. */
    for (idx_t i = last + 1; i <= n; ++i) {
        if (i <= 6) continue;  /* already initialised */

        cache[i] = cache[i - 1]
                 + (cache[i - 2] * (seq_t)4)
                 - cache[i - 3]
                 - (cache[i - 4] * (seq_t)4)
                 + cache[i - 5]
                 + cache[i - 6];
    }
    last = n;

    return cache[n];
}
