/*
 * A154254  a(n) = 9*n^2 - 8*n + 2.
 * https://oeis.org/A154254
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*n^2 - 8*n + 2
 * Range:     u8 n <= 5, u16 n <= 85, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_t = (seq_t)n;          /* Convert index to sequence type */
    seq_t n_sq = n_t * n_t;        /* n^2 */
    return 9 * n_sq - 8 * n_t + 2; /* 9*n^2 - 8*n + 2 */
}
