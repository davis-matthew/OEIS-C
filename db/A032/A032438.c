/*
 * A032438  a(n) = n^2 - floor((n+1)/2)^2.
 * https://oeis.org/A032438
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = n^2 - floor((n+1)/2)^2
 * Range:     u8 n <= 18, u16 n <= 295, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t m  = (seq_t)(n + 1) / 2;
    return nn * nn - m * m;
}
