/*
 * A036659  Product of n with sum of next n consecutive integers.
 * https://oeis.org/A036659
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2*(3*n + 1)/2
 * Range:     u8 n <= 4, u16 n <= 27, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t t = (3 * N + 1);
    seq_t res = N * N * t;
    return res / 2;
}
