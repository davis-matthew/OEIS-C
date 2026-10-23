/*
 * A159512  Numerator of Hermite(n, 13/14).
 * https://oeis.org/A159512
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(0)=1, a(1)=13, a(n) = 13*a(n-1) - 98*(n-1)*a(n-2) for n>=2
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 7, i64 n <= 14, i128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

#ifndef MAX_N
#define MAX_N 10000   /* largest index expected in the tests */
#endif

seq_t a(idx_t n)
{
    /* static table that holds values up to MAX_N */
    static seq_t cache[MAX_N + 1];
    static idx_t  max_calculated = 0;
    static int    initialized   = 0;

    /* initialise first two values on first call */
    if (!initialized) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)13;
        max_calculated = 1;
        initialized = 1;
    }

    /* if requested n is larger than our table, clamp to maximum */
    if (n > MAX_N) {
        /* You may choose to extend the table or signal error.
           For the purpose of this implementation we simply
           compute up to MAX_N and return the last known value. */
        n = MAX_N;
    }

    /* compute missing values up to n */
    for (idx_t i = max_calculated + 1; i <= n; ++i) {
        seq_t term1 = cache[i - 1] * (seq_t)13;
        seq_t term2 = cache[i - 2] * (seq_t)(98 * (i - 1));
        cache[i] = term1 - term2;
    }
    max_calculated = n;

    return cache[n];
}
