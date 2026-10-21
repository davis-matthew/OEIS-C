/*
 * A054668  Number of distinct non-extendable sequences X={x(1),x(2),...,x(k)}
 *          where x(1)=1, the x(i)'s are distinct elements of {1,...,n} with
 *          |x(i)-x(i+1)|=1 or 2, for i=1,2,...,k.
 * https://oeis.org/A054668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=2, a(3)=4, a(4)=8, a(5)=16
   a(n) = 2*a(n-1) - a(n-4)   for n ≥ 6
   This follows from the generating function
   G(x) = (x^4+1)/(x^4-2x+1). */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the first five terms explicitly */
    if (n == 0) return 0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)4;
    if (n == 4) return (seq_t)8;
    if (n == 5) return (seq_t)16;

    /* Initialize a sliding window holding
       a(n-4), a(n-3), a(n-2), a(n-1)  */
    seq_t v0 = (seq_t)2; /* a(2) */
    seq_t v1 = (seq_t)4; /* a(3) */
    seq_t v2 = (seq_t)8; /* a(4) */
    seq_t v3 = (seq_t)16;/* a(5) */
    seq_t ai;

    /* Iterate from 6 up to n */
    for (idx_t i = 6; i <= n; ++i) {
        ai = v3 + v3 - v0;  /* ai = 2*v3 - v0 = 2*a(n-1) - a(n-4) */
        /* Shift the window for the next step */
        v0 = v1;
        v1 = v2;
        v2 = v3;
        v3 = ai;
    }
    return v3;
}
