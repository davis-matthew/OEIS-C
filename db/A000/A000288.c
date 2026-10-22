/*
 * A000288  Tetranacci numbers: a(n) = a(n-1) + a(n-2) + a(n-3) + a(n-4) with
 *          a(0) = a(1) = a(2) = a(3) = 1.
 * https://oeis.org/A000288
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3) + a(n-4), a(0) = a(1) = a(2) =
 *            a(3) = 1
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 35, u64 n <= 69, u128 n <= 137,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t w = 1, x = 1, y = 1, z = 1; /* a(i), a(i+1), a(i+2), a(i+3) */
    idx_t i;
    if (n < 4)
        return (seq_t)1;
    for (i = 3; i < n; i++) {
        seq_t t = w + x + y + z;
        w = x;
        x = y;
        y = z;
        z = t;
    }
    return z;
}
