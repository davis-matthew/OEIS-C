/*
 * A015223  Odd pentagonal pyramidal numbers.
 * https://oeis.org/A015223
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2n+1)*(4n+1)^2
 * Range:     u8 n <= 1, u16 n <= 12, u32 n <= 511, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t two_n_plus1   = sn * 2 + 1;
    seq_t four_n_plus1  = sn * 4 + 1;
    return two_n_plus1 * four_n_plus1 * four_n_plus1;
}
