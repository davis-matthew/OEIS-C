/*
 * A133203  a(n) = a(n-1) + 8*n + 4, n > 2.
 * https://oeis.org/A133203
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*n^2 + 8*n - 29 for n > 1, with a(0) = 0, a(1) = 1
 * Range:     u8 n <= 7, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    seq_t N = (seq_t)n;
    return 4 * N * N + 8 * N - (seq_t)29;
}
