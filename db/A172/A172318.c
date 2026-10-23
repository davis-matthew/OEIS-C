/*
 * A172318  9th column of the array A172119.
 * https://oeis.org/A172318
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A172318: 9th column of A172119
   Recurrence: a(n) = 2*a(n-1) - a(n-9) for n ≥ 9
   Initial values: a(n) = 2^n for 0 ≤ n ≤ 8
   (a(0)=1, a(1)=2, …, a(8)=256). */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the small values directly */
    if (n < (idx_t)9)
        return ((seq_t)1 << (unsigned)n);

    /* Buffer holds a(i-9)..a(i-1) for the current i */
    seq_t buf[9];
    buf[0] = (seq_t)1;                     /* a(0) */
    for (idx_t i = 1; i < 9; ++i)
        buf[i] = buf[i - 1] + buf[i - 1];   /* a(i) = 2*a(i-1) */

    seq_t cur = 0;
    for (idx_t i = 9; i <= n; ++i) {
        /* a(i) = 2*buf[8] - buf[0]  (buf[8] = a(i-1), buf[0] = a(i-9)) */
        cur = buf[8] + buf[8];
        cur = cur - buf[0];

        /* Shift the window left: discard a(i-9), make room for a(i) */
        for (int k = 0; k < 8; ++k)
            buf[k] = buf[k + 1];
        buf[8] = cur;
    }
    return cur;
}
