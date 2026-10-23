/*
 * A147592  Expansion of 1/(1 + x - x^2 - 3*x^3 - x^4 + x^5 + x^6).
 * https://oeis.org/A147592
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 21, i16 n <= 41, i32 n <= 80, i64 n <= 159,
 *            i128 n <= 317, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=-1, a(2)=2, a(3)=0, a(4)=0, a(5)=4
   and for n≥6 : a(n) = -a(n-1) + a(n-2) + 3*a(n-3) + a(n-4) - a(n-5) - a(n-6) */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* initial values */
    static const seq_t init[6] = { (seq_t)1, (seq_t)-1, (seq_t)2,
                                   (seq_t)0, (seq_t)0, (seq_t)4 };

    if (n < 6)
        return init[n];

    /* sliding window for the last six terms */
    seq_t buf[6];
    for (int i = 0; i < 6; ++i)
        buf[i] = init[i];

    /* compute up to n */
    for (idx_t m = 6; m <= n; ++m) {
        seq_t next = -buf[5] + buf[4] + (seq_t)3 * buf[3] + buf[2]
                     - buf[1] - buf[0];
        /* shift left */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = buf[5];
        buf[5] = next;
    }

    return buf[5];
}
