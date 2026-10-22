/*
 * A007518  a(n) = floor(n*(n+2)*(2*n-1)/8).
 * https://oeis.org/A007518
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(n+2)*(2*n-1)/8)
 * Range:     u8 n <= 9, u16 n <= 63, u32 n <= 2422, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    return n_seq * (n_seq + 2) * (2 * n_seq - 1) / 8;
}
