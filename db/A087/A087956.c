/*
 * A087956  a(n) is the square of the n-th partial sum minus the n-th partial
 *          sum of the squares, divided by a(n-1), for all n>=1, starting with
 *          a(0)=1, a(1)=3.
 * https://oeis.org/A087956
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 28, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A087956
   Recurrence:  a(n) = a(n-1) + 3*a(n-2) - a(n-3)   (n > 3)
   Initial terms: a(0) = 1, a(1) = 3, a(2) = 2, a(3) = 11
*/

#include <stdint.h>

seq_t a(idx_t n)
{
    /* handle the explicitly known initial values */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)11;

    /* t0 = a(n-3), t1 = a(n-2), t2 = a(n-1) for the next iteration */
    seq_t t0 = (seq_t)3;  /* a(1) */
    seq_t t1 = (seq_t)2;  /* a(2) */
    seq_t t2 = (seq_t)11; /* a(3) */
    seq_t next;

    for (idx_t i = 4; i <= n; ++i)
    {
        next = t2 + (seq_t)3 * t1 - t0;
        t0 = t1;
        t1 = t2;
        t2 = next;
    }

    return t2;
}
