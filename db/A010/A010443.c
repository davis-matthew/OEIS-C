/*
 * A010443  Squares mod 82.
 * https://oeis.org/A010443
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) is the n-th quadratic residue modulo 82 in increasing order
 * Range:     u8 n <= 42, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t residues[42] = {
        0, 1, 2, 4, 5, 8, 9, 10, 16, 18, 20, 21, 23, 25,
        31, 32, 33, 36, 37, 39, 40, 41, 42, 43, 45, 46,
        49, 50, 51, 57, 59, 61, 62, 64, 66, 72, 73, 74,
        77, 78, 80, 81
    };

    return residues[(size_t)(n - 1) % 42];
}
