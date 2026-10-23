/*
 * A010401  Squares mod 40.
 * https://oeis.org/A010401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) is the periodic sequence of the 9 quadratic residues modulo
 *            40 in increasing order: 0, 1, 4, 9, 16, 20, 24, 25, 36. The
 *            sequence repeats every 9 terms
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t residues[9] = {0, 1, 4, 9, 16, 20, 24, 25, 36};
    return residues[(size_t)((n - 1) % 9)];
}
