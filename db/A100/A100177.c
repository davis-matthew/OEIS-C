/*
 * A100177  Structured meta-prism numbers, the n-th number from a structured
 *          n-gonal prism number sequence.
 * https://oeis.org/A100177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n^4 - 3*n^3 + 4*n^2)/2
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 304, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn   = (seq_t)n;                /* n */
    seq_t n_sq = nn * nn;                 /* n^2 */
    seq_t n4   = n_sq * n_sq;             /* n^4 */

    /* numerator = n^4 - 3*n^3 + 4*n^2 */
    seq_t numerator = n4 - 3 * nn * n_sq + 4 * n_sq;

    return numerator / 2;
}
