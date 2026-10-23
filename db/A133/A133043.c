/*
 * A133043  Number of segments needed to draw the spiral of equilateral
 *          triangles with side lengths which follow the Padovan sequence.
 * https://oeis.org/A133043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1)=3, a(2)=5, a(3)=6; a(n)=n+4 for n>3
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)6;
    return (seq_t)n + (seq_t)4;
}
