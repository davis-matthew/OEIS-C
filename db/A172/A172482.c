/*
 * A172482  a(n) = (1+n)*(9 + 11*n + 4*n^2)/3.
 * https://oeis.org/A172482
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1+n)*(9+11*n+4*n^2)/3
 * Range:     u8 n <= 4, u16 n <= 35, u32 n <= 1474, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t term = N + 1;
    seq_t c = 9 + (seq_t)11 * N + (seq_t)4 * N * N;
    return term * c / 3;
}
