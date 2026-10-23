/*
 * A042089  Denominators of continued fraction convergents to sqrt(568).
 * https://oeis.org/A042089
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 286*a(n-4) - a(n-8) for n>7, with initial values a(0)=1,
 *            a(1)=1, a(2)=5, a(3)=6, a(4)=281, a(5)=287, a(6)=1429, a(7)=1716
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial seven terms */
    static const seq_t init[8] = {
        (seq_t)1,
        (seq_t)1,
        (seq_t)5,
        (seq_t)6,
        (seq_t)281,
        (seq_t)287,
        (seq_t)1429,
        (seq_t)1716
    };

    if (n < 8)
        return init[n];

    /* Circular buffer of the last eight computed values */
    seq_t buf[8];
    size_t i;
    for (i = 0; i < 8; ++i)
        buf[i] = init[i];

    seq_t tmp;
    for (i = 8; i <= n; ++i) {
        size_t idx4 = (i - 4) & 7;   /* index for a(i-4) */
        size_t idx8 = (i - 8) & 7;   /* index for a(i-8) */

        tmp = (seq_t)286 * buf[idx4] - buf[idx8];
        buf[i & 7] = tmp;
    }

    return buf[n & 7];
}
