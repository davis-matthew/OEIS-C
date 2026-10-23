/*
 * A131640  First differences are periodic: 50, 50, 75, 50, 50, 75, ...
 * https://oeis.org/A131640
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 985 + 50*n + 25*floor(n/3)
 * Range:     u8 none, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    return 985 + 50*n_seq + 25*(n_seq/3);
}
