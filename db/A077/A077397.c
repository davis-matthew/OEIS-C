/*
 * A077397  Expansion of (1+31*x-2*x^2-2*x^3)/(1-16*x^2+x^4).
 * https://oeis.org/A077397
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1; a(1)=31; a(2)=14; a(3)=494;
   for n≥4, a(n)=16*a(n-2)-a(n-4). */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the first four terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)31;
    if (n == 2) return (seq_t)14;
    if (n == 3) return (seq_t)494;

    /* Keep a sliding window of the last four values:
         a0 = a(i-4), a1 = a(i-3), a2 = a(i-2), a3 = a(i-1)
       Initially for i = 4 this window is:
         a0 = 1, a1 = 31, a2 = 14, a3 = 494. */
    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)31;
    seq_t a2 = (seq_t)14;
    seq_t a3 = (seq_t)494;
    seq_t curr = 0;

    for (idx_t i = 4; i <= n; ++i) {
        /* a(i) = 16*a(i-2) - a(i-4) */
        curr = (seq_t)((seq_t)16 * a2 - a0);

        /* Slide the window for the next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = curr;
    }

    return curr;
}
