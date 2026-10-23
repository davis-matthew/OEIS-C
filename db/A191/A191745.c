/*
 * A191745  a(n) = 12*n^3 + 9*n^2 + 2*n.
 * https://oeis.org/A191745
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*n^3 + 9*n^2 + 2*n
 * Range:     u8 n <= 2, u16 n <= 17, u32 n <= 709, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t n_sq  = n_seq * n_seq;                    // n^2
    seq_t inner = (seq_t)12 * n_sq + (seq_t)9 * n_seq + (seq_t)2;
    return n_seq * inner;                           // n*(12n^2 + 9n + 2)
}
