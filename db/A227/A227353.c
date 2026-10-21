/*
 * A227353  Number of lattice points in the closed region bounded by the
 *          graphs of y = 3*x/5, x = n, and y = 0, excluding points on the
 *          x-axis.
 * https://oeis.org/A227353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(3*n-1)/10)
 * Range:     u8 n <= 9, u16 n <= 195, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t prod = nn * (nn * 3 - 1);
    return prod / 10;
}
