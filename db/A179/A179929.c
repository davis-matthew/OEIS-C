/*
 * A179929  a(n) = 2^n*A(n, -1/2), A(n, x) the Eulerian polynomials.
 * https://oeis.org/A179929
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 3*a(n-1) - Σ_{k=0}^{n-1} binom(n-1,k)*a(k)*a(n-1-k) a(0)
 *            = 1 Recurrence from OEIS (Peter Bala)
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 12, i64 n <= 21, i128 n <= 35,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 199 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

seq_t a(idx_t n)
{
    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated size */
    static idx_t  last  = 0;   /* highest index computed */

    /* Allocate (or grow) cache to hold indices up to n */
    if (cache == NULL) {
        cap = (size_t)(n + 1);                     /* initial capacity */
        cache = seq_alloc(cap);                    /* allocate array */
        cache[0] = 1;                              /* a(0) */
        last = 0;
    } else if ((size_t)n >= cap) {
        size_t new_cap = (size_t)(n + 1);
        cache = seq_realloc(cache, cap, new_cap);
        cap = new_cap;                             /* update capacity */
    }

    /* Compute new terms up to n if needed */
    for (idx_t i = last + 1; i <= n; ++i) {
        if (i == 0) continue;                     /* already initialized */
        seq_t sum = 0;                            /* Σ term = binom* a(k)*a(i-1-k) */
        seq_t bin = 1;                            /* binom(i-1,0) */

        idx_t m = i - 1;                          /* use m = i-1 for brevity */
        for (idx_t k = 0; k <= m; ++k) {
            /* term = binom(m,k) * a(k) * a(m-k) */
            seq_t t = bin;
            t = t * cache[k];
            t = t * cache[m - k];
            sum += t;

            /* update binom(m,k+1) = binom(m,k) * (m-k) / (k+1) */
            if (k < m) {
                seq_t num = (seq_t)(m - k);
                seq_t den = (seq_t)(k + 1);
                bin = bin * num / den;
            }
        }

        /* Recurrence: a(i) = 3*a(i-1) - sum */
        seq_t three = (seq_t)3;
        cache[i] = three * cache[i - 1] - sum;
    }

    last = n;
    return cache[n];
}
