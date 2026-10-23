/*
 * A025445  Expansion of 1/((1-2*x)*(1-3*x)*(1-4*x)*(1-7*x)).
 * https://oeis.org/A025445
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*a(n-1) - 89*a(n-2) + 206*a(n-3) - 168*a(n-4) Initial
 *            values: a(0)=1, a(1)=16, a(2)=167, a(3)=1454
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the first four terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)16;
    if (n == 2) return (seq_t)167;
    if (n == 3) return (seq_t)1454;

    /* Use a rolling window of the last four values to compute the next term. */
    seq_t a0 = (seq_t)1;    /* a(k-4) */
    seq_t a1 = (seq_t)16;   /* a(k-3) */
    seq_t a2 = (seq_t)167;  /* a(k-2) */
    seq_t a3 = (seq_t)1454; /* a(k-1) */
    seq_t next;

    for (idx_t i = 4; i <= n; ++i) {
        /* Compute the next term as per the recurrence,
           avoiding intermediate underflow. */
        seq_t sum1 = (seq_t)16 * a3 + (seq_t)206 * a1;
        seq_t sum2 = (seq_t)89 * a2 + (seq_t)168 * a0;
        next = sum1 - sum2;

        /* Shift the window. */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }

    return a3;
}
