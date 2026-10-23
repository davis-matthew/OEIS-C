/*
 * A152813  a(n) = 2*n^2 + 10*n + 3.
 * https://oeis.org/A152813
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n^2 + 10*n + 3
 * Range:     u8 n <= 9, u16 n <= 178, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    return (seq_t)2 * N * N + (seq_t)10 * N + (seq_t)3;
}
