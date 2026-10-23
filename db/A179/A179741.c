/*
 * A179741  a(n) = (2*n+1)*(6*n-1).
 * https://oeis.org/A179741
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (2*n+1)*(6*n-1)
 * Range:     i8 n <= 3, i16 n <= 52, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    return (2*nn + 1)*(6*nn - 1);
}
