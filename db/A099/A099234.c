/*
 * A099234  A trisection of 1/(1-x-x^4).
 * https://oeis.org/A099234
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1)+3*a(n-2)+3*a(n-3)+a(n-4) Initial values: a(0)=1,
 *            a(1)=1, a(2)=4, a(3)=10 This recurrence is linear and can be
 *            evaluated on the fly in O(n) time without any allocation. The
 *            implementation works for any seq_t (plain integer or
 *            arbitrary‑precision) and for calls in arbitrary order
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 46, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the first four terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)10;

    /* Use four registers to keep the last four values of the
       recurrence.  Initialise them with the base cases */
    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)1;   /* a(1) */
    seq_t a2 = (seq_t)4;   /* a(2) */
    seq_t a3 = (seq_t)10;  /* a(3) */
    seq_t an;              /* placeholder for a(n) */

    /* Compute a(k) for k = 4 … n */
    for (idx_t k = 4; k <= n; ++k) {
        an = a3            /* a(k-1) */
           + (seq_t)3 * a2 /* 3*a(k-2) */
           + (seq_t)3 * a1 /* 3*a(k-3) */
           + a0;          /* a(k-4) */

        /* Roll the window of the last four values */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }

    return an;
}
