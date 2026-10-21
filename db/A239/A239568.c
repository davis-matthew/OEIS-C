/*
 * A239568  Number of ways to place 2 points on a triangular grid of side n so
 *          that they are not adjacent.
 * https://oeis.org/A239568
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)*(n-2)*(n+5)/8
 * Range:     u8 n <= 6, u16 n <= 26, u32 n <= 295, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return nn * (nn - 1u) * (nn - 2u) * (nn + 5u) / 8u;
}
