/*
 * A176712  a(n) = 16*n^4 + 256*n^3 + 1160*n^2 + 1088*n + 285.
 * https://oeis.org/A176712
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*n^4 + 256*n^3 + 1160*n^2 + 1088*n + 285
 * Range:     u8 none, u16 n <= 4, u32 n <= 124, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N  = (seq_t)n;
    seq_t N2 = N * N;
    seq_t N3 = N2 * N;
    seq_t N4 = N3 * N;
    return 16 * N4 + 256 * N3 + 1160 * N2 + 1088 * N + 285;
}
