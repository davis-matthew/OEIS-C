/*
 * A002411  Pentagonal pyramidal numbers: a(n) = n^2*(n+1)/2.
 * https://oeis.org/A002411
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, changed
 * Formula:   a(n) = n^2*(n+1)/2
 * Range:     u8 n <= 7, u16 n <= 50, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t triangular = n_seq * (n_seq + 1) / 2;
    return n_seq * triangular;
}
