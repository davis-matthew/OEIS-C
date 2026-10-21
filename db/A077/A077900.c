/*
 * A077900  Expansion of (1-x)^(-1)/(1+x-2*x^2+2*x^3).
 * https://oeis.org/A077900
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 3*a(n-2) - 4*a(n-3) + 2*a(n-4) for n >= 4, with initial
 *            values a(0)=1, a(1)=0, a(2)=3, a(3)=-4
 * Range:     i8 n <= 7, i16 n <= 13, i32 n <= 27, i64 n <= 32, i128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *vals = NULL;
    static idx_t capacity = 0;
    static idx_t size    = 0;   /* number of entries already computed (>=0) */

    /* initialize the array with the first four terms */
    if (capacity == 0) {
        capacity = 4;
        vals = (seq_t*)malloc(capacity * sizeof(seq_t));
        vals[0] = (seq_t)1;
        vals[1] = (seq_t)0;
        vals[2] = (seq_t)3;
        vals[3] = (seq_t)-4;
        size = 4;
    }

    /* if the requested index is already computed, just return it */
    if (n < size)
        return vals[n];

    /* grow the allocated space if necessary */
    if (n >= capacity) {
        idx_t newcap = capacity;
        while (newcap <= n) newcap *= 2;
        vals = (seq_t*)realloc(vals, newcap * sizeof(seq_t));
        capacity = newcap;
    }

    /* compute new terms up to n */
    for (idx_t i = size; i <= n; ++i) {
        /* recurrence holds for i >= 4; for i < 4 the values are already set */
        vals[i] = (seq_t)3 * vals[i-2] - (seq_t)4 * vals[i-3] + (seq_t)2 * vals[i-4];
    }

    size = n + 1;   /* update the computed size */
    return vals[n];
}
