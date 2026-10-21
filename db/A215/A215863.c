/*
 * A215863  Number of simple labeled graphs on n+3 nodes with exactly n
 *          connected components that are trees or cycles.
 * https://oeis.org/A215863
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(n+3,4)*(n^2+13*n+24)/2
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 72, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t binom = n_seq * (n_seq + 1) * (n_seq + 2) * (n_seq + 3) / 24;
    seq_t poly = n_seq * n_seq + 13 * n_seq + 24;
    return binom * poly / 2;
}
