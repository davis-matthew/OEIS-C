/*
 * A173960  Averages of four consecutive odd squares.
 * https://oeis.org/A173960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n^2 + 8*n + 9
 * Range:     u8 n <= 6, u16 n <= 126, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    return 4 * n_seq * n_seq + 8 * n_seq + 9;
}
