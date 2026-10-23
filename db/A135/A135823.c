/*
 * A135823  Moves to permute (ufr,dfr)(ubr,ubl) corners in a 3 X 3 X 3 Rubik's
 *          Cube.
 * https://oeis.org/A135823
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) = 21 if n even, 9261 if n odd
 * Range:     u8 n <= 0, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (n & 1) ? (seq_t)9261 : (seq_t)21;
}
