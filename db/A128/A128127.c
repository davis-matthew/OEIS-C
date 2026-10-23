/*
 * A128127  The number of regular pentagons found by constructing n
 *          equally-spaced points on each side of the pentagon and drawing
 *          lines parallel to the pentagon side.
 * https://oeis.org/A128127
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  more, nonn
 * Formula:   a(n) = (10*n^2 - 4*n + 5 - (-1)^n)/4
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn   = (seq_t)n;
    seq_t sign = (n & 1) ? (seq_t)-1 : (seq_t)1;
    seq_t num  = 10 * nn * nn - 4 * nn + 5 - sign; /* numerator */
    return num / 4; /* integer division */
}
