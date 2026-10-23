/*
 * A144390  a(n) = 3*n^2 - n - 1.
 * https://oeis.org/A144390
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n^2 - n - 1
 * Range:     u8 n <= 9, u16 n <= 147, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    return 3 * n_seq * n_seq - n_seq - 1;
}
