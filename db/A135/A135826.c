/*
 * A135826  Moves to permute (ufl,ubr)(ufr,ubl) corners in a 3 X 3 X 3 Rubik's
 *          cube.
 * https://oeis.org/A135826
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) = 28 if n is even, 196 if n is odd
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 3, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return ((n & 1) == 0) ? (seq_t)28 : (seq_t)196;
}
