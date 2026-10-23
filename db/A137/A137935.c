/*
 * A137935  a(n) = 5n + 26*floor(n/5).
 * https://oeis.org/A137935
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*n + 26*floor(n/5)
 * Range:     u8 n <= 25, u16 n <= 6425, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t q = (seq_t)(n / 5);
    return 5 * n_seq + 26 * q;
}
