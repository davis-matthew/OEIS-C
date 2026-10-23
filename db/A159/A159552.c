/*
 * A159552  Numerator of Hermite(n, 7/18).
 * https://oeis.org/A159552
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(0)=1, a(1)=7, a(n)=7*a(n-1)-162*(n-1)*a(n-2) for n>=2
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 6, i64 n <= 13, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes the OEIS sequence A159552 (numerators of Hermite(n,7/18)*9^n) */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;    /* cache of computed values */
    static size_t cap   = 0;       /* allocated capacity */
    static size_t sz    = 0;       /* number of values computed (cache[0..sz-1] are valid) */

    /* If cache is empty, initialise with space for at least two terms */
    if (cache == NULL) {
        cap = (n + 1 >= 2) ? (n + 1) : 2;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)7;
        sz = 2;                     /* we have a(0) and a(1) stored */
    }

    /* Grow the cache if necessary */
    if ((idx_t)(n + 1) > (idx_t)cap) {
        size_t new_cap = cap;
        while ((idx_t)(n + 1) > (idx_t)new_cap)
            new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cap   = new_cap;
    }

    /* Return cached value if already known */
    if (n < sz) {
        return cache[n];
    }

    /* Compute missing terms up to n using the recurrence */
    for (idx_t i = sz; i <= n; ++i) {
        seq_t a_im1 = cache[i - 1];
        seq_t a_im2 = cache[i - 2];

        /* term = 7 * a(i-1) - 162*(i-1) * a(i-2) */
        seq_t mult = (seq_t)162 * (seq_t)(i - 1);
        seq_t term = (seq_t)7 * a_im1 - mult * a_im2;

        cache[i] = term;
    }

    sz = n + 1;         /* we now have the first n+1 terms stored */
    return cache[n];
}
