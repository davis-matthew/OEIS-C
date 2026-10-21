/*
 * A218456  a(n) = 2*n^3 - 313*n^2 + 6823*n - 13633.
 * https://oeis.org/A218456
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2*n^3 - 313*n^2 + 6823*n - 13633
 * Range:     i8 none, i16 n <= 29, i32 n <= 33, i64 n <= 33, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    return (seq_t)2 * n3 - (seq_t)313 * n2 + (seq_t)6823 * nn - (seq_t)13633;
}
