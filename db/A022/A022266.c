/*
 * A022266  a(n) = n*(9*n - 1)/2.
 * https://oeis.org/A022266
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(9*n - 1)/2
 * Range:     u8 n <= 7, u16 n <= 120, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t res = N * ((N * 9) - 1) / 2;
    return res;
}
