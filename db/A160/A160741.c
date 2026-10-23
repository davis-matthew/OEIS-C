/*
 * A160741  Numerator of P_6(2n), the Legendre polynomial of order 6 at 2n.
 * https://oeis.org/A160741
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac, easy
 * Formula:   a(n) = -5 + 420*n^2 - 5040*n^4 + 14784*n^6
 * Range:     i8 n <= 0, i16 n <= 1, i32 n <= 7, i64 n <= 292, i128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N   = (seq_t)n;
    seq_t n2  = N * N;          /* n^2 */
    seq_t n4  = n2 * n2;        /* n^4 */
    seq_t n6  = n4 * n2;        /* n^6 */
    seq_t res = - (seq_t)5;
    res += (seq_t)420 * n2;
    res -= (seq_t)5040 * n4;
    res += (seq_t)14784 * n6;
    return res;
}
