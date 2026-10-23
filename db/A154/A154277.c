/*
 * A154277  a(n) = 81*n^2 - 72*n + 17.
 * https://oeis.org/A154277
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 81*n^2 - 72*n + 17
 * Range:     u8 n <= 2, u16 n <= 28, u32 n <= 7282, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    return 81 * N * N - 72 * N + 17;
}
