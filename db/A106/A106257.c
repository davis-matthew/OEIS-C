/*
 * A106257  Numbers k such that k^2 = 12*n^2 + 13.
 * https://oeis.org/A106257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A106257: Numbers k such that k^2 = 12*n^2 + 13.
   Recurrence: a(1)=5, a(2)=11, a(3)=59, a(4)=149,
   a(n)=14*a(n-2) - a(n-4)   for n>=5. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the first four terms directly. */
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)11;
    if (n == 3) return (seq_t)59;
    if (n == 4) return (seq_t)149;

    /* Circular buffer to keep the last four values:
       a[0] = a(n-4), a[1] = a(n-3), a[2] = a(n-2), a[3] = a(n-1). */
    seq_t a0 = (seq_t)5;   /* a(1) */
    seq_t a1 = (seq_t)11;  /* a(2) */
    seq_t a2 = (seq_t)59;  /* a(3) */
    seq_t a3 = (seq_t)149; /* a(4) */

    for (idx_t i = 5; i <= n; ++i) {
        /* Compute next term using 128‑bit intermediate to avoid
           overflow in 64‑bit builds. */
        seq_t next = (seq_t)(((__uint128_t)14 * a2) - a0);

        /* Shift the buffer for the next iteration. */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
