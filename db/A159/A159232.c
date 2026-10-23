/*
 * A159232  Numerator of Hermite(n, 4/9).
 * https://oeis.org/A159232
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(0)=1, a(1)=8, a(n+1) = 8*a(n) - 162*n*a(n-1) (n>=1)
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 6, i64 n <= 13, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This is derived from the Hermite recurrence and yields
    the exact numerator of Hermite(n,4/9) multiplied by 9^n. */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for previously computed values */
    static seq_t *cache = NULL;
    static size_t cap  = 0;
    static size_t len  = 0;          /* number of entries stored (index+1) */

    /* initialise cache on first invocation */
    if (!cache) {
        cap = 4;                     /* initial capacity */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* allocation failed; unlikely in tests */
        cache[0] = (seq_t)1;          /* a(0) */
        cache[1] = (seq_t)8;          /* a(1) */
        len = 2;                      /* we have values up to index 1 */
    }

    /* grow the cache if the requested index is beyond current capacity */
    if ((size_t)n >= cap) {
        size_t new_cap = cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* realloc failed */
        cache = tmp;
        cap = new_cap;
    }

    /* if the value is already cached, return it immediately */
    if ((size_t)n < len) return cache[n];

    /* compute terms from the first missing up to n */
    for (idx_t i = len; i <= n; ++i) {
        if (i <= 1) continue;          /* a(0) and a(1) are already set */

        /* a(i) = 8*a(i-1) - 162*(i-1)*a(i-2);   (i>=2) */
        seq_t prod = (seq_t)162 * (seq_t)(i - 1) * cache[i - 2];
        cache[i] = (seq_t)8 * cache[i - 1] - prod;
    }

    len = (size_t)n + 1;
    return cache[n];
}
