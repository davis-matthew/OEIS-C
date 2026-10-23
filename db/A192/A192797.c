/*
 * A192797  Molecular topological indices of the cycle graphs.
 * https://oeis.org/A192797
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(2*n^2+(-1)^n+15)/4
 * Range:     u8 n <= 7, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast index to seq_t */
    seq_t nn = (seq_t)n;
    /* compute 2*n^2 */
    seq_t two_n_sq = nn * nn * 2;
    /* compute the additive constant */
    seq_t add = (n & 1) ? (seq_t)14 : (seq_t)16;
    /* now val = 2*n^2 + 14 (odd) or +16 (even) */
    seq_t val = two_n_sq + add;
    /* final result: n * val / 4 */
    return nn * val / 4;
}
