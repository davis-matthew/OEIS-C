/*
 * A069099  Centered heptagonal numbers.
 * https://oeis.org/A069099
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (7*n^2 - 7*n + 2)/2
 * Range:     u8 n <= 9, u16 n <= 137, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    return (7 * N * N - 7 * N + 2) / 2;
}
