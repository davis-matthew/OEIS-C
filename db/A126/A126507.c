/*
 * A126507  Number of base 12 n-digit numbers with adjacent digits differing
 *          by four or less.
 * https://oeis.org/A126507
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 8*a(n-1) - 18*a(n-3) + 2*a(n-4) + 6*a(n-5) for n >= 6,
 *            with initial values a(0)=1, a(1)=12, a(2)=88, a(3)=672,
 *            a(4)=5168, a(5)=39796
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Base values */
    static const seq_t init[6] = {
        (seq_t)1,   /* a(0) */
        (seq_t)12,  /* a(1) */
        (seq_t)88,  /* a(2) */
        (seq_t)672, /* a(3) */
        (seq_t)5168,/* a(4) */
        (seq_t)39796/* a(5) */
    };

    if (n < 6)
        return init[n];

    /* Sliding window of the last five terms:
       cur[0] = a(n-5), cur[1] = a(n-4), ..., cur[4] = a(n-1) */
    seq_t cur[5];
    cur[0] = init[1];
    cur[1] = init[2];
    cur[2] = init[3];
    cur[3] = init[4];
    cur[4] = init[5];

    for (idx_t i = 6; i <= n; ++i)
    {
        seq_t next = (seq_t)8 * cur[4]
                   - (seq_t)18 * cur[2]
                   + (seq_t)2  * cur[1]
                   + (seq_t)6  * cur[0];

        /* Shift window */
        cur[0] = cur[1];
        cur[1] = cur[2];
        cur[2] = cur[3];
        cur[3] = cur[4];
        cur[4] = next;
    }

    return cur[4];
}
