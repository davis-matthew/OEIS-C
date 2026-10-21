/*
 * A077876  Expansion of (1-x)^(-1)/(1-x+2*x^2).
 * https://oeis.org/A077876
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 13, i16 n <= 31, i32 n <= 45, i64 n <= 45, i128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A077876: a(n) = (1-x)^(-1)/(1-x+2x^2).  Recurrence from
   denominator 1-2x+3x^2-2x^3:  a(n)=2*a(n-1)-3*a(n-2)+2*a(n-3)
   for n>=3, with a(0)=1, a(1)=2, a(2)=1. */
#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache for memoisation.  We grow it as needed. */
    static seq_t *cache = NULL;
    static size_t cap = 0;      /* allocated size */
    static size_t size = 0;     /* number of terms stored (>=3 after init) */

    /* Ensure we have room for up to n (inclusive). */
    size_t required = (size_t)n + 1;
    if (cap < required) {
        size_t newcap = cap ? cap * 2 : 16;
        if (newcap < required) newcap = required;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();      /* cannot happen in typical use */
        cache = tmp;
        cap = newcap;
    }

    /* Initialize first three terms if not already done. */
    if (size < 3) {
        /* a(0) = 1, a(1) = 2, a(2) = 1 */
        if (size <= 0) cache[0] = (seq_t)1;
        if (size <= 1) cache[1] = (seq_t)2;
        if (size <= 2) cache[2] = (seq_t)1;
        size = 3;
    }

    /* Compute terms lazily up to n. */
    for (size_t i = size; i < required; ++i) {
        seq_t t1 = cache[i - 1];
        seq_t t2 = cache[i - 2];
        seq_t t3 = cache[i - 3];
        /* a(i) = 2*a(i-1) - 3*a(i-2) + 2*a(i-3) */
        cache[i] = (seq_t)2 * t1 - (seq_t)3 * t2 + (seq_t)2 * t3;
    }
    size = required;
    return cache[(size_t)n];
}
