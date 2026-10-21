/*
 * A086640  Arrange n^2 octagons that each have area 7 so that they leave
 *          (n-1)^2 square gaps each with area 2; a(n) is the total area of
 *          these polygons.
 * https://oeis.org/A086640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 9*n*n - 4*n + 2
 * Range:     u8 n <= 5, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return nn*nn*9 - nn*4 + 2;
}
