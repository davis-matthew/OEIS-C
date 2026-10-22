/*
 * A007533  a(n) = (5*n + 1)^2 + 4*n + 1.
 * https://oeis.org/A007533
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 25*n^2 + 14*n + 2
 * Range:     u8 n <= 2, u16 n <= 50, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n;
    return 25 * m * m + 14 * m + 2;
}
