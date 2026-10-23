/*
 * A192465  Constant term of the reduction by x^2->x+2 of the polynomial
 *          p(n,x)=1+x^n+x^(2n).
 * https://oeis.org/A192465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 7*a(n-2) - 6*a(n-3) + 8*a(n-4) for n ≥ 5 with
 *            initial values a(0)=0, a(1)=3, a(2)=9, a(3)=25, a(4)=93 (a(0) is
 *            used only inside the recurrence)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t  cached = 0;   /* highest index computed so far */
    static size_t capacity = 0; /* allocated capacity of cache */

    /* initialise cache on first call */
    if (cache == NULL) {
        capacity = 5;                       /* store indices 0..4 */
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        cache[0] = (seq_t)0;                /* a(0) */
        cache[1] = (seq_t)3;                /* a(1) */
        cache[2] = (seq_t)9;                /* a(2) */
        cache[3] = (seq_t)25;               /* a(3) */
        cache[4] = (seq_t)93;               /* a(4) */
        cached = 4;
    }

    /* extend the cache if needed */
    if (n > cached) {
        /* grow capacity (double) until it fits the requested index */
        while (capacity <= (size_t)n) {
            capacity <<= 1;
        }
        cache = (seq_t *)realloc(cache, capacity * sizeof(seq_t));

        /* compute new terms using the recurrence */
        for (idx_t i = cached + 1; i <= n; ++i) {
            /* a(i) = 6*a(i-1) - 7*a(i-2) - 6*a(i-3) + 8*a(i-4) */
            cache[i] = (seq_t)6 * cache[i - 1]
                     - (seq_t)7 * cache[i - 2]
                     - (seq_t)6 * cache[i - 3]
                     + (seq_t)8 * cache[i - 4];
        }
        cached = n;
    }

    return cache[n];
}
