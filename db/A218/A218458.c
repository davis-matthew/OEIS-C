/*
 * A218458  a(n) = 2*n^3 - 163*n^2 + 2777*n - 11927.
 * https://oeis.org/A218458
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2*n^3 - 163*n^2 + 2777*n - 11927
 * Range:     i8 none, i16 n <= 33, i32 n <= 33, i64 n <= 33, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t sn = (seq_t)n;
    return 2*sn*sn*sn - 163*sn*sn + 2777*sn - 11927;
}
