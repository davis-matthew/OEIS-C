/*
 * A076808  a(n) = 82n^3 - 1228n^2 + 6130n - 5861.
 * https://oeis.org/A076808
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 82*n^3 - 1228*n^2 + 6130*n - 5861
 * Range:     i8 none, i16 n <= 12, i32 n <= 33, i64 n <= 33, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t res = 82;      /* coefficient of n^3 */
    res = res * nn - 1228;
    res = res * nn + 6130;
    res = res * nn - 5861;
    return res;
}
