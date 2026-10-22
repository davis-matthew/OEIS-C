/*
 * A001971  Nearest integer to n^2/8.
 * https://oeis.org/A001971
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n^2 + 4) / 8)
 * Range:     u8 n <= 45, u16 n <= 724, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    return (n_seq * n_seq + 4) / 8;
}
