/*
 * A042009  Denominators of continued fraction convergents to sqrt(527).
 * https://oeis.org/A042009
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy, less
 * Formula:   a(n) = 1056*a(n-4) - a(n-8) for n > 7 with initial values
 *            a(0)=1, a(1)=1, a(2)=22, a(3)=23, a(4)=1034, a(5)=1057,
 *            a(6)=23231, a(7)=24288
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t init[8] = { (seq_t)1, (seq_t)1, (seq_t)22, (seq_t)23,
                                   (seq_t)1034, (seq_t)1057, (seq_t)23231, (seq_t)24288 };
    if (n < 8)
        return init[(size_t)n];

    seq_t buf[8];
    for (size_t i = 0; i < 8; ++i)
        buf[i] = init[i];

    for (idx_t i = 8; i <= n; ++i) {
        /* Compute 1056 * buf[4] without overflow */
        seq_t mult = buf[4];
        for (int k = 0; k < 1055; ++k)
            mult += buf[4];                /* 1056 * buf[4] */
        seq_t next = mult - buf[0];

        /* shift buffer left by one */
        for (int j = 0; j < 7; ++j)
            buf[j] = buf[j + 1];
        buf[7] = next;
    }

    return buf[7];
}
