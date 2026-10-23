/*
 * A021479  Decimal expansion of 1/475.
 * https://oeis.org/A021479
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = a(n-1) - a(n-9) + a(n-10) for n > 11;
 *            a(0)=0,a(1)=0,a(2)=2,a(3)=1,a(4)=0,a(5)=5,a(6)=2,a(7)=6,
 *            a(8)=3,a(9)=1,a(10)=5,a(11)=7
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t init[12] = {
    (seq_t)0, (seq_t)0, (seq_t)2, (seq_t)1, (seq_t)0, (seq_t)5,
    (seq_t)2, (seq_t)6, (seq_t)3, (seq_t)1, (seq_t)5, (seq_t)7
};

seq_t a(idx_t n)
{
    if (n < 12)
        return init[(size_t)n];

    /* Buffer holding a(i-10) … a(i-1) when the loop is at i. */
    seq_t buf[10];
    for (idx_t i = 0; i < 10; ++i)
        buf[i] = init[(size_t)(i + 2)];   /* a(2) … a(11) */

    seq_t val;
    for (idx_t i = 12; i <= n; ++i) {
        /* Compute a(i) = a(i-1) - a(i-9) + a(i-10) without unsigned underflow:
           a(i) = (a(i-1) + a(i-10)) - a(i-9). */
        val = buf[9] + buf[0];
        val = val - buf[1];

        /* Shift buffer left, make room for the new term. */
        for (idx_t j = 0; j < 9; ++j)
            buf[j] = buf[j + 1];
        buf[9] = val;
    }

    return buf[9];   /* a(n) */
}
