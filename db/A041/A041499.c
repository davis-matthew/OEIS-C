/*
 * A041499  Denominators of continued fraction convergents to sqrt(266).
 * https://oeis.org/A041499
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 1370*a(n-4) - a(n-8) for n>7; initial values: a(0)=1,
 *            a(1)=3, a(2)=13, a(3)=42, a(4)=1357, a(5)=4113, a(6)=17809,
 *            a(7)=57540
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Initial eight terms */
    static const seq_t init[8] = {
        (seq_t)1,      /* a(0) */
        (seq_t)3,      /* a(1) */
        (seq_t)13,     /* a(2) */
        (seq_t)42,     /* a(3) */
        (seq_t)1357,   /* a(4) */
        (seq_t)4113,   /* a(5) */
        (seq_t)17809,  /* a(6) */
        (seq_t)57540   /* a(7) */
    };

    if (n < 8)
        return init[(size_t)n];

    /* Circular buffer of the last ten terms (size 8 is enough) */
    seq_t buf[8];
    for (size_t i = 0; i < 8; ++i)
        buf[i] = init[i];

    seq_t val;
    /* Iterate from 8 up to n, using the recurrence. */
    for (idx_t i = 8; i <= n; ++i) {
        /* indices modulo 8 */
        size_t idx4 = (size_t)(i - 4) % 8;
        size_t idx8 = (size_t)(i - 8) % 8;
        val = ((seq_t)1370 * buf[idx4]) - buf[idx8];
        buf[(size_t)i % 8] = val;
    }

    return buf[(size_t)n % 8];
}
