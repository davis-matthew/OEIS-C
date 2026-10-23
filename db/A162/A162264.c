/*
 * A162264  a(n) = (2*n^3 + 5*n^2 + 7*n)/2.
 * https://oeis.org/A162264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n^3 + 5*n^2 + 7*n)/2
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t n2 = n_seq * n_seq;
    seq_t n3 = n2 * n_seq;
    seq_t val = (2 * n3 + 5 * n2 + 7 * n_seq) / 2;
    return val;
}
