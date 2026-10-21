/*
 * A228220  Number of second differences of arrays of length 5 of numbers in
 *          0..n.
 * https://oeis.org/A228220
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 20*n^3 + 9*n^2 + n + 1
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t n2 = n_seq * n_seq;
    seq_t n3 = n2 * n_seq;
    seq_t term = (seq_t)20 * n3 + (seq_t)9 * n2 + n_seq + (seq_t)1;
    return term;
}
