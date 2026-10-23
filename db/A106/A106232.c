/*
 * A106232  Least k > 0 such that (4*n^2 + 2)*(k^2) + (4*n^2 + 2)*k + 1 = j^2.
 * https://oeis.org/A106232
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 4 ; for n > 1, a(n) = 2*n^2 - 2*n
 * Range:     u8 n <= 11, u16 n <= 181, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)4;
    seq_t n_sq = (seq_t)n * (seq_t)n;          /* n^2 */
    seq_t twice_sq = (seq_t)2 * n_sq;          /* 2*n^2 */
    seq_t twice_n  = (seq_t)2 * (seq_t)n;      /* 2*n */
    return twice_sq - twice_n;                /* 2*n^2 - 2*n */
}
