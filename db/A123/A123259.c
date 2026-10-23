/*
 * A123259  a(n) = A123228(n)/2.
 * https://oeis.org/A123259
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -13*a(n-1) - 74*a(n-2) - 74*a(n-3) - 13*a(n-4) - a(n-5).
 *            Initial values: a(0)=3, a(1)=-7, a(2)=11, a(3)=233, a(4)=-3357
 * Range:     i8 n <= 2, i16 n <= 5, i32 n <= 11, i64 n <= 21, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the first five terms directly */
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)-7;
    if (n == 2) return (seq_t)11;
    if (n == 3) return (seq_t)233;
    if (n == 4) return (seq_t)-3357;

    /* Allocate space for the terms up to n */
    seq_t *v = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!v) abort();          /* out of memory */

    /* Initialize the first five terms */
    v[0] = (seq_t)3;
    v[1] = (seq_t)-7;
    v[2] = (seq_t)11;
    v[3] = (seq_t)233;
    v[4] = (seq_t)-3357;

    /* Compute the remaining terms using the recurrence */
    for (idx_t i = 5; i <= n; ++i) {
        v[i] = -(seq_t)13 * v[i-1]
               - (seq_t)74 * v[i-2]
               - (seq_t)74 * v[i-3]
               - (seq_t)13 * v[i-4]
               - v[i-5];
    }

    seq_t result = v[n];
    free(v);
    return result;
}
