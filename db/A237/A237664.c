/*
 * A237664  Interpolation polynomial through n+1 points (0,1), (1,1), ...,
 *          (n-1,1) and (n,n) evaluated at 2n.
 * https://oeis.org/A237664
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1) * binom(2n, n) + 1, a(0) = 0 where binom(2n, n) =
 *            (2n)!/(n! * n!)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    seq_t binom = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t numerator   = (seq_t)n + (seq_t)i;   /* n + i   */
        seq_t denominator = (seq_t)i;              /* i       */
        binom = binom * numerator / denominator;
    }

    seq_t term = (seq_t)(n - 1) * binom + (seq_t)1;
    return term;
}
