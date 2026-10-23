/*
 * A125202  a(n) = 4*n^2 - 6*n + 1.
 * https://oeis.org/A125202
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 4*n*n - 6*n + 1
 * Range:     i8 n <= 6, i16 n <= 91, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Convert the index to the sequence type */
    seq_t tn = (seq_t)n;
    /* Compute 4*n*n - 6*n + 1 using seq_t arithmetic */
    return 4 * tn * tn - 6 * tn + 1;
}
