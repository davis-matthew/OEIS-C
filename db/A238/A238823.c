/*
 * A238823  a(n) = 3*a(n-1)-4*a(n-3)+a(n-4)+a(n-5)+3*a(n-6)-a(n-7) for n >= 8,
 *          with initial values as shown.
 * https://oeis.org/A238823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 25, u64 n <= 50, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A238823
   Recurrence: a(n) = 3*a(n-1) - 4*a(n-3) + a(n-4) + a(n-5) + 3*a(n-6) - a(n-7)
   for n >= 8, with initial values
   a(1)=2, a(2)=3, a(3)=6, a(4)=14, a(5)=34, a(6)=84, a(7)=208
   (a(0) is unused).  The computation is performed iteratively, using
   a fixed ring buffer of the last seven terms, so no dynamic memory
   allocation is required.  The function works for any integer types
   seq_t and idx_t supplied by the build. */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Hard‑coded initial terms */
    static const seq_t init[7] = { (seq_t)2, (seq_t)3, (seq_t)6,
                                   (seq_t)14, (seq_t)34, (seq_t)84,
                                   (seq_t)208 };

    if (n <= 7)
        return init[n - 1];

    /* Buffers to hold the last seven terms:
       buf[0] = a(i-7), buf[1] = a(i-6), ..., buf[6] = a(i-1)  */
    seq_t buf[7];
    for (int i = 0; i < 7; ++i)
        buf[i] = init[i];

    seq_t cur;
    for (idx_t i = 8; i <= n; ++i)
    {
        /* Compute a(i) using the recurrence */
        cur = (seq_t)3 * buf[6]
            - (seq_t)4 * buf[4]
            + buf[3]
            + buf[2]
            + (seq_t)3 * buf[1]
            - buf[0];

        /* Shift the buffer: drop a(i-7), bring in a(i) */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = buf[5];
        buf[5] = buf[6];
        buf[6] = cur;
    }

    return cur;
}
