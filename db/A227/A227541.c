/*
 * A227541  a(n) = floor(13*n^2/4).
 * https://oeis.org/A227541
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(13*n^2/4)
 * Range:     u8 n <= 4, u16 n <= 72, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;          // cast index to seq_t
    seq_t sq = tn * tn;           // n*n
    seq_t prod = sq * (seq_t)13;  // 13*n^2
    seq_t result = prod / (seq_t)4; // floor division by 4
    return result;
}
