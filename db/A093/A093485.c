/*
 * A093485  a(n) = (27*n^2 + 9*n + 2)/2.
 * https://oeis.org/A093485
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (27*n^2 + 9*n + 2)/2
 * Range:     u8 n <= 2, u16 n <= 68, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t val = 27 * N * N + 9 * N + 2;
    return val / 2;
}
