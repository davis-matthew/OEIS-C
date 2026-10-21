/*
 * A236340  Number of length n binary words such that maximal runs of 1's are
 *          restricted to length one or two and maximal runs of 0's are of odd
 *          length.
 * https://oeis.org/A236340
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=2, a(2)=3, a(3)=5, a(4)=7; recurrence:
 *            a(n)=2*a(n-2)+a(n-3) for n>=5
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the initial values */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)5;
    if (n == 4) return (seq_t)7;

    /* Circular buffer of the previous five terms:
       buf[0]=a(n-5), buf[1]=a(n-4), buf[2]=a(n-3),
       buf[3]=a(n-2), buf[4]=a(n-1)   */
    seq_t buf[5];
    buf[0] = (seq_t)1;   /* a(0) */
    buf[1] = (seq_t)2;   /* a(1) */
    buf[2] = (seq_t)3;   /* a(2) */
    buf[3] = (seq_t)5;   /* a(3) */
    buf[4] = (seq_t)7;   /* a(4) */

    seq_t next = 0;
    for (idx_t i = 5; i <= n; ++i)
    {
        /* a(i) = 2*a(i-2)+a(i-3) */
        next = (seq_t)2 * buf[3] + buf[2];

        /* shift the buffer left by one position */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = next;
    }
    return next;
}
