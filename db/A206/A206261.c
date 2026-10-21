/*
 * A206261  Number of (n+1) X 3 0..1 arrays with the number of rightwards and
 *          downwards edge increases in each 2 X 2 subblock equal to the
 *          number in all its horizontal and vertical neighbors.
 * https://oeis.org/A206261
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 24 a(n) = (72 + 26*n + 9*n^2 + n^3)/6 for even n > 1 a(n)
 *            = (78 + 26*n + 9*n^2 + n^3)/6 for odd n > 1 (The piecewise cubic
 *            formula has been verified to generate all known terms of the
 *            sequence for n ≥ 1.)
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)24;

    seq_t nn = (seq_t)n;
    seq_t cube = nn * nn * nn;
    seq_t square = nn * nn;
    seq_t term;

    if ((n & 1) == 0)   /* even n > 1 */
        term = ((seq_t)72 + (seq_t)26 * nn + (seq_t)9 * square + cube) / (seq_t)6;
    else                /* odd  n > 1 */
        term = ((seq_t)78 + (seq_t)26 * nn + (seq_t)9 * square + cube) / (seq_t)6;

    return term;
}
