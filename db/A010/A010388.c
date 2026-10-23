/*
 * A010388  Squares mod 26.
 * https://oeis.org/A010388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = the nth smallest quadratic residue modulo 26, i.e. the
 *            nth element of the set {0,1,3,4,9,10,12,13,14,16,17,22,23,25}
 *            sorted ascending. The sequence is periodic with period 14
 * Range:     u8 n <= 14, u16 n <= 14, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t residues[14] = {
    0, 1, 3, 4, 9, 10, 12, 13, 14, 16, 17, 22, 23, 25
};

seq_t a(idx_t n)
{
    idx_t idx = (n - 1) % 14;        /* n starts at 1 */
    return residues[idx];
}
