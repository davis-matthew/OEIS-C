/*
 * A195316  Centered 36-gonal numbers.
 * https://oeis.org/A195316
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 18*n^2 - 18*n + 1
 * Range:     u8 n <= 4, u16 n <= 60, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    return 18 * N * N - 18 * N + 1;
}
