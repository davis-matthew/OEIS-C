/*
 * A034262  a(n) = n^3 + n.
 * https://oeis.org/A034262
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 + n
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1625, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = n;
    return N * N * N + N;
}
