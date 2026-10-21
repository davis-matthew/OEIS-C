/*
 * A205345  Number of length 8 nonnegative integer arrays starting and ending
 *          with 0 with adjacent elements unequal but differing by no more
 *          than n.
 * https://oeis.org/A205345
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) - 21*a(n-2) + 35*a(n-3) - 35*a(n-4) + 21*a(n-5)
 *            - 7*a(n-6) + a(n-7), a(1)=0, a(2)=272, a(3)=3272, a(4)=18652,
 *            a(5)=71630, a(6)=214724, a(7)=542850
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 31, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is computed by repeated application of the linear recurrence.
 * No extra memory or caching is required; the routine works for any order of
 * calls and for any idx_t >= 1.  All intermediate sums are kept non‑negative,
 * so an unsigned seq_t can be used safely. */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* initial seven terms */
    static const seq_t init[7] = {
        (seq_t)0,
        (seq_t)272,
        (seq_t)3272,
        (seq_t)18652,
        (seq_t)71630,
        (seq_t)214724,
        (seq_t)542850
    };

    if (n <= 7)
        return init[n - 1];

    /* buffer holding the last seven values: buf[0] = a(k-7), ..., buf[6] = a(k-1) */
    seq_t buf[7];
    for (size_t i = 0; i < 7; ++i)
        buf[i] = init[i];

    for (idx_t k = 8; k <= n; ++k) {
        /* positive part: 7*buf[6] + 35*buf[4] + 21*buf[2] + buf[0] */
        seq_t positive = (seq_t)7 * buf[6] + (seq_t)35 * buf[4]
                       + (seq_t)21 * buf[2] + buf[0];
        /* negative part: 21*buf[5] + 35*buf[3] + 7*buf[1] */
        seq_t negative = (seq_t)21 * buf[5] + (seq_t)35 * buf[3]
                       + (seq_t)7  * buf[1];
        /* next value */
        seq_t next = positive - negative;

        /* shift buffer one position to the left */
        for (size_t i = 0; i < 6; ++i)
            buf[i] = buf[i + 1];
        buf[6] = next;
    }

    return buf[6];
}
