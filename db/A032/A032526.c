/*
 * A032526  a(n) = floor(5*n^2/2).
 * https://oeis.org/A032526
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(5*n^2/2)
 * Range:     u8 n <= 10, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t sq = nn * nn;
    return ((seq_t)5 * sq) / (seq_t)2;
}
