/*
 * A176382  x-values in the solution to x^2-73*y^2=1.
 * https://oeis.org/A176382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A176382: x-values in the solution to x^2-73*y^2=1
   Recurrence: a(1)=1, a(2)=2281249, a(n)=4562498*a(n-1)-a(n-2) for n>=3
   (computed iteratively, no global state needed) */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 1)
        return (seq_t)1;

    if (n == 2)
        return (seq_t)2281249;

    const seq_t C = (seq_t)4562498;
    seq_t prev2 = (seq_t)1;        /* a(n-2)  */
    seq_t prev1 = (seq_t)2281249;  /* a(n-1)  */
    seq_t curr;

    for (idx_t i = 3; i <= n; ++i) {
        curr = C * prev1 - prev2;  /* a(n) = C*a(n-1) - a(n-2) */
        prev2 = prev1;
        prev1 = curr;
    }

    return prev1;
}
