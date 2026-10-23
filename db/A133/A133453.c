/*
 * A133453  a(n) = 3*a(n-1) - a(n-3) + 3*a(n-4), with initial values
 *          2,4,13,40.
 * https://oeis.org/A133453
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - a(n-3) + 3*a(n-4) Initial values: a(0)=2,
 *            a(1)=4, a(2)=13, a(3)=40
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    seq_t v[4] = {(seq_t)2, (seq_t)4, (seq_t)13, (seq_t)40};

    if (n < 4)
        return v[(size_t)n];

    for (idx_t i = 4; i <= n; ++i) {
        seq_t new_val = 3 * v[3] - v[1] + 3 * v[0];
        v[0] = v[1];
        v[1] = v[2];
        v[2] = v[3];
        v[3] = new_val;
    }
    return v[3];
}
