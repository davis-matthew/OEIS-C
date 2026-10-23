/*
 * A022280  a(n) = n*(23*n - 1)/2.
 * https://oeis.org/A022280
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(23*n - 1)/2
 * Range:     u8 n <= 4, u16 n <= 75, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;               /* cast index to seq_t */
    seq_t term = N * ((seq_t)23 * N - 1) / 2;
    return term;
}
