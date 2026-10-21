/*
 * A217748  Number of regions with infinite area in the exterior of a regular
 *          n-gon with all diagonals drawn.
 * https://oeis.org/A217748
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-3) for n>3; a(3) = 1
 * Range:     u8 n <= 17, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3) return 0;
    if (n == 3) return 1;
    seq_t sn = (seq_t)n;
    return sn * (sn - 3);
}
