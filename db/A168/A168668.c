/*
 * A168668  a(n) = n*(2 + 5*n).
 * https://oeis.org/A168668
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(2 + 5*n)
 * Range:     u8 n <= 6, u16 n <= 114, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    return n_seq * ((seq_t)2 + (seq_t)5 * n_seq);
}
