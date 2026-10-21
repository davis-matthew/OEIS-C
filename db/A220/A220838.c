/*
 * A220838  Tropical version of Somos-4 sequence A006720.
 * https://oeis.org/A220838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = max(a(n-1)+a(n-3), 2*a(n-2)) - a(n-4); a(1) = -1, a(2)=0,
 *            a(3)=0, a(4)=0
 * Range:     i8 n <= 34, i16 n <= 724, i32 n <= 5000, i64 n <= 5000,
 *            i128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Compute terms of OEIS A220838 */
seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache = NULL;      /* array of terms, index 0 unused */
    static size_t capacity = 0;      /* allocated array size */
    static size_t length   = 0;      /* number of terms stored (indices 0..length-1) */

    /* initialise cache on first call */
    if (cache == NULL) {
        capacity = 8;                               /* initial capacity */
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache) return (seq_t)0;                 /* allocation failed */

        /* store initial values (a(1) .. a(4)) */
        cache[0] = (seq_t)0;                        /* unused index 0 */
        cache[1] = (seq_t)-1;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)0;
        cache[4] = (seq_t)0;

        length = 5;                                 /* terms [0..4] are valid */
    }

    /* grow the cache if needed */
    if (n >= capacity) {
        size_t new_cap = capacity;
        while (new_cap <= (size_t)n) new_cap *= 2;

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return cache[n];                  /* return what we have on failure */
        cache = tmp;
        capacity = new_cap;
    }

    /* return already computed value if present */
    if (n < length) return cache[n];

    /* compute all missing terms up to n */
    for (idx_t i = (idx_t)length; i <= n; ++i) {
        seq_t a1 = cache[i-1];
        seq_t a3 = cache[i-3];
        seq_t a2 = cache[i-2];
        seq_t a4 = cache[i-4];

        seq_t t1 = a1 + a3;
        seq_t t2 = a2 * 2;
        seq_t mx = (t1 > t2) ? t1 : t2;

        cache[i] = mx - a4;
    }

    length = (size_t)n + 1;          /* update number of stored terms */
    return cache[n];
}
