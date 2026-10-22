/*
 * A007715  Number of 5-leaf rooted trees with n levels.
 * https://oeis.org/A007715
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(5*n^2+n+6)/24
 * Range:     u8 n <= 5, u16 n <= 23, u32 n <= 224, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t poly = 5 * n_seq * n_seq + n_seq + 6;
    return n_seq * (n_seq + 1) * poly / 24;
}
