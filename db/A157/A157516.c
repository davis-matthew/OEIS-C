/*
 * A157516  a(n) = 5000*n^2 - 200*n + 1.
 * https://oeis.org/A157516
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5000*n^2 - 200*n + 1
 * Range:     u8 none, u16 n <= 3, u32 n <= 926, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    return 5000 * N * N - 200 * N + 1;
}
