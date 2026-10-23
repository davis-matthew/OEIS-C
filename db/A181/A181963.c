/*
 * A181963  Prime-generating polynomial: a(n) = 25*n^2 - 1185*n + 14083.
 * https://oeis.org/A181963
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 25*n^2 - 1185*n + 14083
 * Range:     u8 none, u16 n <= 74, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    return (seq_t)25 * N * N - (seq_t)1185 * N + (seq_t)14083;
}
