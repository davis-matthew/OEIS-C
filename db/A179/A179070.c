/*
 * A179070  a(1)=a(2)=a(3)=1, a(4)=3; thereafter a(n) = a(n-1) + a(n-3).
 * https://oeis.org/A179070
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + a(n-3), a(1)=a(2)=a(3)=1, a(4)=3
 * Range:     u8 n <= 16, u16 n <= 30, u32 n <= 59, u64 n <= 117,
 *            u128 n <= 233, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS sequence A179070.
 * Works for any n >= 1, independent of call order, and runs in O(n). */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n = 0, return 0 */
    if (n <= 3) return (seq_t)1;     /* a(1)=a(2)=a(3)=1 */

    /* initialise first four terms */
    seq_t a1 = (seq_t)1; /* a(1) */
    seq_t a2 = (seq_t)1; /* a(2) */
    seq_t a3 = (seq_t)1; /* a(3) */
    seq_t a4 = (seq_t)3; /* a(4) */

    if (n == 4) return a4;

    /* iterate from i = 5 to n */
    seq_t an = 0;
    for (idx_t i = 5; i <= n; ++i) {
        an = a4 + a2;   /* a(i-1) + a(i-3) */
        /* shift variables for next iteration */
        a2 = a3;        /* a(i-3) becomes a(i-2) for next step */
        a3 = a4;        /* a(i-2) becomes a(i-1)          */
        a4 = an;        /* a(i-1) becomes a(i)            */
    }

    return an;
}
