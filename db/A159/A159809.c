/*
 * A159809  Positive numbers y such that y^2 is of the form x^2+(x+223)^2 with
 *          integer x.
 * https://oeis.org/A159809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-3) - a(n-6); a(1)=197, a(2)=223, a(3)=257,
 *            a(4)=925, a(5)=1115, a(6)=1345
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 30, u64 n <= 69, u128 n <= 144,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Return early for the initial terms */
    static const seq_t init[6] = {197, 223, 257, 925, 1115, 1345};

    if (n <= 6 && n >= 1)
        return init[(size_t)(n - 1)];

    /* Ring buffer holding the six most recent terms:
       buffer[0] = a(i-6), buffer[1] = a(i-5), ... , buffer[5] = a(i-1) */
    seq_t buffer[6];
    for (size_t i = 0; i < 6; ++i)
        buffer[i] = init[i];

    /* Compute terms up to n */
    for (idx_t i = 7; i <= n; ++i) {
        /* a(i) = 6 * a(i-3) - a(i-6) */
        seq_t next = 6 * buffer[3] - buffer[0];

        /* Shift left: drop a(i-6) and make room for a(i) */
        for (size_t j = 0; j < 5; ++j)
            buffer[j] = buffer[j + 1];
        buffer[5] = next;
    }

    return buffer[5];
}
