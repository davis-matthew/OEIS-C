/*
 * A239608  a(n) = sin( arcsin(n)- 2*arccos(n) )^2.
 * https://oeis.org/A239608
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2*(4*n^2-3)^2
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 25, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t n_sq = N * N;        /* n^2 */
    seq_t t   = 4 * n_sq - 3;  /* 4*n^2 - 3 */
    return n_sq * t * t;       /* n^2 * (4*n^2-3)^2 */
}
