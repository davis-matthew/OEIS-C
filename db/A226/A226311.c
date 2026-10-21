/*
 * A226311  a(n+5) = a(n+4)+a(n+3)+a(n+2)+a(n+1)+2*a(n) with a(0)=2, a(1)=1,
 *          a(2)=5, a(3)=10, a(4)=20.
 * https://oeis.org/A226311
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n+5) = a(n+4)+a(n+3)+a(n+2)+a(n+1)+2*a(n);
   initial values: a(0)=2, a(1)=1, a(2)=5, a(3)=10, a(4)=20 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Return the initial values directly. */
    static const seq_t init[5] = {2, 1, 5, 10, 20};
    if (n < 5) return init[n];

    /* Buffer holds the last five terms: a(n-4) … a(n). */
    seq_t buf[5];
    for (idx_t i = 0; i < 5; ++i) buf[i] = init[i];

    for (idx_t i = 5; i <= n; ++i) {
        /* a(i) = a(i-1)+a(i-2)+a(i-3)+a(i-4)+2*a(i-5) */
        seq_t val = buf[4] + buf[3] + buf[2] + buf[1] + buf[0] + buf[0];
        /* Shift the window for the next iteration. */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = val;
    }
    return buf[4];
}
