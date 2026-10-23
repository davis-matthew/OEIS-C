/*
 * A038709  a(n) = floor(n*(n+1/2)/4).
 * https://oeis.org/A038709
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(n + 1/2)/4) = floor(n*(2n+1)/8)
 * Range:     u8 n <= 31, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    return (N * (2 * N + (seq_t)1)) / (seq_t)8;
}
