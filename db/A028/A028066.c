/*
 * A028066  Expansion of 1/((1-3x)(1-5x)(1-8x)(1-11x)).
 * https://oeis.org/A028066
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (5*11^(n+3)-2*8^(n+4)+4*5^(n+4)-3^(n+5))/720
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)27;
    if (n == 2) return (seq_t)474;
    if (n == 3) return (seq_t)6902;

    /* recurrence: a(n) = 27*a(n-1) - 255*a(n-2) + 989*a(n-3) - 1320*a(n-4) */
    seq_t p0 = (seq_t)6902;   /* a(n-1) */
    seq_t p1 = (seq_t)474;    /* a(n-2) */
    seq_t p2 = (seq_t)27;     /* a(n-3) */
    seq_t p3 = (seq_t)1;      /* a(n-4) */
    seq_t cur = 0;

    for (idx_t i = 4; i <= n; ++i) {
        cur = ((seq_t)27)  * p0
            - ((seq_t)255) * p1
            + ((seq_t)989) * p2
            - ((seq_t)1320)* p3;

        /* shift the window */
        p3 = p2;
        p2 = p1;
        p1 = p0;
        p0 = cur;
    }

    return cur;
}
