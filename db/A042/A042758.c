/*
 * A042758  Numerators of continued fraction convergents to sqrt(910).
 * https://oeis.org/A042758
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 362*a(n-2) - a(n-4) Initial values: a(0)=30, a(1)=181,
 *            a(2)=10890, a(3)=65521
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the first four terms directly */
    if (n < 4) {
        static const seq_t base[4] = {30, 181, 10890, 65521};
        return base[(int)n];
    }

    /* Sliding window of the last four computed terms */
    seq_t buf[4] = {30, 181, 10890, 65521};

    for (idx_t i = 4; i <= n; ++i) {
        unsigned int idx  = (unsigned int)(i & 3);        /* i % 4      */
        unsigned int idx2 = (unsigned int)((i - 2) & 3);  /* (i-2) % 4  */
        unsigned int idx4 = (unsigned int)((i - 4) & 3);  /* (i-4) % 4  */

        buf[idx] = (seq_t)362 * buf[idx2] - buf[idx4];
    }

    return buf[(unsigned int)(n & 3)];
}
