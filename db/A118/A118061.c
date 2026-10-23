/*
 * A118061  a(n) = 9800*n^2-5740*n-4059.
 * https://oeis.org/A118061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 9800*n^2 - 5740*n - 4059 (offset 1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 662, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    return 9800 * N * N - 5740 * N - 4059;
}
