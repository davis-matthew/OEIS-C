/*
 * A010387  Squares mod 25.
 * https://oeis.org/A010387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the nth quadratic residue modulo 25 in ascending order
 * Range:     u8 n <= 11, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
seq_t a(idx_t n)
{
    static const seq_t vals[11] = {0, 1, 4, 6, 9, 11, 14, 16, 19, 21, 24};
    return vals[(size_t)((n - 1) % 11)];
}
