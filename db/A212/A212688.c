/*
 * A212688  Number of (w,x,y,z) with all terms in {1,...,n} and
 *          2|w-x|>=n+|y-z|.
 * https://oeis.org/A212688
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - a(n-2) - 5*a(n-3) + 5*a(n-4) + a(n-5) -
 *            3*a(n-6) + a(n-7) Initial values: a(0)=0, a(1)=0, a(2)=4,
 *            a(3)=14, a(4)=44, a(5)=98, a(6)=200
 * Range:     u8 n <= 6, u16 n <= 26, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* initial values for n = 0 … 6 */
    static const seq_t init[7] = {
        (seq_t)0,   /* a(0) */
        (seq_t)0,   /* a(1) */
        (seq_t)4,   /* a(2) */
        (seq_t)14,  /* a(3) */
        (seq_t)44,  /* a(4) */
        (seq_t)98,  /* a(5) */
        (seq_t)200  /* a(6) */
    };

    if (n < 7)
        return init[n];

    /* circular buffer holding last 7 computed values */
    seq_t buf[7];
    for (idx_t i = 0; i < 7; ++i)
        buf[i] = init[i];

    /* compute a(7) … a(n) iteratively */
    for (idx_t i = 7; i <= n; ++i) {
        /* indices of the 7 previous terms */
        idx_t i1 = (i - 1) % 7;
        idx_t i2 = (i - 2) % 7;
        idx_t i3 = (i - 3) % 7;
        idx_t i4 = (i - 4) % 7;
        idx_t i5 = (i - 5) % 7;
        idx_t i6 = (i - 6) % 7;
        idx_t i7 = (i - 7) % 7;

        /* compute using only addition and subtraction to avoid negative intermediates */
        seq_t pos = (seq_t)3 * buf[i1] + (seq_t)5 * buf[i4]
                    + buf[i5] + buf[i7];
        seq_t neg = buf[i2] + (seq_t)5 * buf[i3] + (seq_t)3 * buf[i6];
        buf[i % 7] = pos - neg;   /* result is non‑negative by the recurrence */
    }

    return buf[n % 7];
}
