/*
 * A098921  Let [n] = {1,2,...,n}. Let G_n be the union of all closed line
 *          segments joining any two elements of [n] X [n] along a vertical or
 *          horizontal line, or along a line with slope +-1. Then a(n) =
 *          combined total of the number of (nondegenerate) triangles and
 *          rectangles for which all edges are subsets o...
 * https://oeis.org/A098921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n^4 - 2*n^3 - 5*n^2 - 22*n + (n%2?18:12)) / 12 Note:
 *            For n even the constant 12 yields an integer result, for n odd
 *            18 does. The formula can also be written as the floor of the
 *            second expression for all n
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not defined for the sequence, but safe */
    seq_t sn = (seq_t)n;
    seq_t n2 = sn * sn;            /* n² */
    seq_t n3 = n2 * sn;            /* n³ */
    seq_t n4 = n2 * n2;            /* n⁴ */
    seq_t constant = (n & 1) ? (seq_t)18 : (seq_t)12;
    seq_t numerator = 11 * n4
                     - 2 * n3
                     - 5 * n2
                     - 22 * sn
                     + constant;
    return numerator / (seq_t)12;
}
