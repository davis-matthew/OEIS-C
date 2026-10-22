/*
 * A007588  Stella octangula numbers: a(n) = n*(2*n^2 - 1).
 * https://oeis.org/A007588
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = n*(2*n^2 - 1)
 * Range:     u8 n <= 5, u16 n <= 32, u32 n <= 1290, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n;
    seq_t m2 = m * m;
    return m * (2 * m2 - 1);
}
