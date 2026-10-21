/*
 * A083086  a(n) = (2^(n+1) + (-4)^n)/3.
 * https://oeis.org/A083086
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (2^(n+1) + (-4)^n) / 3 Recurrence used: a(0)=1, a(1)=0,
 *            a(n) = -2*a(n-1) + 8*a(n-2) for n ≥ 2 This implementation
 *            memoises computed terms to handle arbitrary call order
 *            efficiently
 * Range:     i8 n <= 4, i16 n <= 8, i32 n <= 16, i64 n <= 32, i128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t size   = 0;   /* number of terms stored in cache */
    static idx_t cap    = 0;   /* current allocated capacity */

    /* initialise cache with the first two terms */
    if (cache == NULL) {
        cap   = 4;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) abort();               /* allocation failure */
        cache[0] = (seq_t)1;                      /* a(0) */
        cache[1] = (seq_t)0;                      /* a(1) */
        size   = 2;
    }

    /* return already computed term */
    if (n < size)
        return cache[n];

    /* expand cache up to term n */
    if (n >= cap) {
        while (cap <= n) cap <<= 1;               /* double until large enough */
        seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
        if (tmp == NULL) abort();                 /* allocation failure */
        cache = tmp;
    }

    /* compute missing terms using the recurrence */
    for (idx_t i = size; i <= n; ++i) {
        seq_t prev1 = cache[i - 1];
        seq_t prev2 = cache[i - 2];
        seq_t term = prev1 * (seq_t)(-2);         /* -2*a(i-1) */
        seq_t tmp  = prev2 * (seq_t)8;            /* 8*a(i-2)  */
        term += tmp;                              /* a(i) = -2*a(i-1)+8*a(i-2) */
        cache[i] = term;
    }

    size = n + 1;      /* cache now contains terms 0..n */
    return cache[n];
}
