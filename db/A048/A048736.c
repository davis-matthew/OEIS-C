/*
 * A048736  Dana Scott's sequence: a(n) = (a(n-2) + a(n-1) * a(n-3)) / a(n-4),
 *          a(0) = a(1) = a(2) = a(3) = 1.
 * https://oeis.org/A048736
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 10*a(n-3) - 10*a(n-6) + a(n-9), a(0) = a(1) = a(2) = a(3)
 *            = 1, a(4) = 2, a(5) = 3, a(6) = 5, a(7) = 13, a(8) = 22
 * Range:     u8 n <= 11, u16 n <= 18, u32 n <= 33, u64 n <= 64, u128 n <= 125,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Let we keep the last 9 values:  a[0] = a(i),
                                     a[1] = a(i-1), …,
                                     a[8] = a(i-8)   */
    if (n <= 3) return (seq_t)1;
    if (n == 4) return (seq_t)2;
    if (n == 5) return (seq_t)3;
    if (n == 6) return (seq_t)5;
    if (n == 7) return (seq_t)13;
    if (n == 8) return (seq_t)22;

    /* initialise the circular buffer with a(8) … a(0) */
    seq_t a_buf[9] = {
        (seq_t)22, /* a(8) */
        (seq_t)13, /* a(7) */
        (seq_t)5,  /* a(6) */
        (seq_t)3,  /* a(5) */
        (seq_t)2,  /* a(4) */
        (seq_t)1,  /* a(3) */
        (seq_t)1,  /* a(2) */
        (seq_t)1,  /* a(1) */
        (seq_t)1   /* a(0) */
    };

    seq_t cur;
    for (idx_t i = 9; i <= n; ++i) {
        /* a(i) = 10*a(i-3) - 10*a(i-6) + a(i-9) */
        cur = a_buf[2] * (seq_t)10  /* a(i-3)  */
            - a_buf[5] * (seq_t)10  /* a(i-6)  */
            + a_buf[8];             /* a(i-9)  */

        /* shift the buffer to incorporate the new term */
        for (int j = 8; j > 0; --j)
            a_buf[j] = a_buf[j-1];
        a_buf[0] = cur;
    }

    return a_buf[0];  /* a(n) */
}
