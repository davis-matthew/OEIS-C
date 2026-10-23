/*
 * A015237  a(n) = (2*n - 1)*n^2.
 * https://oeis.org/A015237
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n - 1)*n^2
 * Range:     u8 n <= 5, u16 n <= 32, u32 n <= 1290, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t two_s_minus_one = (seq_t)(2 * sn - (seq_t)1);
    seq_t n_sq = sn * sn;
    return two_s_minus_one * n_sq;
}
