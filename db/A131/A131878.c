/*
 * A131878  a(n) = 7*n^2 + 14*n + 1.
 * https://oeis.org/A131878
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*n^2 + 14*n + 1
 * Range:     u8 n <= 5, u16 n <= 95, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t result = 7 * n_seq * n_seq + 14 * n_seq + 1;
    return result;
}
