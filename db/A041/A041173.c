/*
 * A041173  Denominators of continued fraction convergents to sqrt(96).
 * https://oeis.org/A041173
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 98*a(n-4) - a(n-8) Initial values:
 *            a(0)=1,a(1)=1,a(2)=4,a(3)=5,a(4)=94,a(5)=99,a(6)=391,a(7)=490
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 19, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Small recurrence table for the first eight terms */
    static const seq_t init[8] = {1, 1, 4, 5, 94, 99, 391, 490};

    if (n < 8)
        return init[(size_t)n];

    /* Circular buffer of the last eight computed terms */
    seq_t buf[8];
    for (size_t j = 0; j < 8; ++j)
        buf[j] = init[j];

    for (idx_t i = 8; i <= n; ++i) {
        seq_t v4 = buf[(i - 4) & 7];
        seq_t v8 = buf[(i - 8) & 7];
        buf[i & 7] = (seq_t)98 * v4 - v8;
    }

    return buf[n & 7];
}
