/*
 * A208222  a(n) = (a(n-1)^3*a(n-3)^2+a(n-2))/a(n-4) with
 *          a(0)=a(1)=a(2)=a(3)=1.
 * https://oeis.org/A208222
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (a(n-1)^3 * a(n-3)^2 + a(n-2)) / a(n-4) with a(0) = a(1)
 *            = a(2) = a(3) = 1
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 7, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Compute a(n) according to the recurrence, memoizing results */
seq_t a(idx_t n) {
    /* static cache */
    static seq_t *cache = NULL;
    static idx_t max_cached = 0;
    static size_t cap = 0;
    static bool inited = false;

    /* initialize cache for the first four terms */
    if (!inited) {
        cap = 4;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        for (idx_t i = 0; i < 4; ++i)
            cache[i] = 1;
        max_cached = 3;
        inited = true;
    }

    /* trivial case */
    if (n < 4)
        return 1;

    /* grow cache if necessary */
    if (n > max_cached) {
        size_t new_cap = cap;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cap = new_cap;

        /* compute new terms */
        for (idx_t i = max_cached + 1; i <= n; ++i) {
            seq_t term = cache[i - 1];
            term *= cache[i - 1];
            term *= cache[i - 1];
            term *= cache[i - 3];
            term *= cache[i - 3];
            term += cache[i - 2];
            term /= cache[i - 4];
            cache[i] = term;
        }
        max_cached = n;
    }

    return cache[n];
}
