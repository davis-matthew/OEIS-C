/*
 * A011587  Legendre symbol (n,29).
 * https://oeis.org/A011587
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) = Legendre symbol (n,29)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const int residues[14] = {
    1, 4, 5, 6, 7, 9, 13, 16,
    20,22,23,24,25,28
};

seq_t a(idx_t n)
{
    uint64_t r = n % 29U;
    if (r == 0) return 0;

    /* Check if r is a quadratic residue modulo 29 */
    switch (r) {
        case 1: case 4: case 5: case 6: case 7:
        case 9: case 13: case 16: case 20:
        case 22: case 23: case 24: case 25:
        case 28:
            return 1;
        default:
            return -1;
    }
}
