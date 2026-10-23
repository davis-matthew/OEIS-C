/*
 * A143045  G.f. A(x) satisfies A(x) = x + A(-x)^2.
 * https://oeis.org/A143045
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) satisfies a(1)=1 and for n≥2, * a(n) = (-1)^n ·
 *            Σ_{i=1}^{n-1} a(i)·a(n−i)
 * Range:     i8 n <= 7, i16 n <= 12, i32 n <= 22, i64 n <= 41, i128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses memoization to answer queries in any order. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;     /* array of computed a values (0‑based) */
    static idx_t  top  = 0;         /* number of terms already filled   */
    static size_t capacity = 0;     /* allocated size of cache          */

    if (n == 0) {                   /* unused offset, return 0 safely   */
        return (seq_t)0;
    }

    /* Grow cache if necessary */
    if (n > top) {
        if ((size_t)n > capacity) {
            size_t new_cap = (size_t)n;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) {              /* allocation failed – return 0      */
                return (seq_t)0;
            }
            cache = tmp;
            capacity = new_cap;
        }

        /* Compute missing terms sequentially */
        for (idx_t i = top + 1; i <= n; ++i) {
            if (i == 1) {            /* a(1) = 1                         */
                cache[0] = (seq_t)1;
                continue;
            }
            seq_t sum = (seq_t)0;
            for (idx_t j = 1; j < i; ++j) {
                sum += cache[j - 1] * cache[i - j - 1];
            }
            seq_t sign = (i & 1ULL) ? (seq_t)-1 : (seq_t)1;
            cache[i - 1] = sign * sum;
        }
        top = n;
    }
    return cache[n - 1];
}
