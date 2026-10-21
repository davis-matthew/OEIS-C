/*
 * A061289  Consider a network of triangles consisting of an equilateral
 *          triangle divided into n^2 equilateral triangles plus a circle
 *          connecting the vertices of the main triangle. Sequence gives
 *          minimal number of corner turns required to trace the network in
 *          one continuous line.
 * https://oeis.org/A061289
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, more
 * Formula:   a(n) = ⌊7*n/2⌋
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return ((seq_t)n * 7) / 2;
}
