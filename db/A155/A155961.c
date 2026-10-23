/*
 * A155961  Numbers whose square can be expressed as the sum of two positive
 *          cubes in at least 3 ways.
 * https://oeis.org/A155961
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, more, bref
 * Formula:   a(n) = 3343221000 * n^3
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t cube = nn * nn * nn;   /* compute n^3 */
    return (seq_t)3343221000u * cube;
}
