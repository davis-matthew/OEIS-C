/*
 * A041829  Denominators of continued fraction convergents to sqrt(435).
 * https://oeis.org/A041829
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 292*a(n-4) - a(n-8) for n>7 Initial values:
 *            a(0)=1,a(1)=1,a(2)=6,a(3)=7,
 *            a(4)=286,a(5)=293,a(6)=1751,a(7)=2044
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t init_vals[8] = {
    (seq_t)1, (seq_t)1, (seq_t)6, (seq_t)7,
    (seq_t)286, (seq_t)293, (seq_t)1751, (seq_t)2044
};

seq_t a(idx_t n)
{
    if (n < 8) {
        return init_vals[(size_t)n];
    }

    seq_t buf[8];
    for (size_t i = 0; i < 8; ++i) {
        buf[i] = init_vals[i];
    }

    for (idx_t i = 8; i <= n; ++i) {
        /* recurrence: a(i) = 292*a(i-4) - a(i-8) */
        seq_t next = (seq_t)292 * buf[(i - 4) % 8] - buf[(i - 8) % 8];
        buf[i % 8] = next;
    }

    return buf[n % 8];
}
