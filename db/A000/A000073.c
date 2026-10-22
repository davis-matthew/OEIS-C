/*
 * A000073  Tribonacci numbers: a(n) = a(n-1) + a(n-2) + a(n-3) for n >= 3
 *          with a(0) = a(1) = 0 and a(2) = 1.
 * https://oeis.org/A000073
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, changed
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3), a(0) = 0, a(1) = 0, a(2) = 1
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 39, u64 n <= 75, u128 n <= 148,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t x = 0, y = 0, z = 1;   /* a(i), a(i+1), a(i+2) */
    idx_t i;
    for (i = 0; i < n; i++) {
        seq_t t = x + y + z;
        x = y;
        y = z;
        z = t;
    }
    return x;
}
