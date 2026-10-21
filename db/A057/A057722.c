/*
 * A057722  a(n) = n^4 - 3*n^2 + 1.
 * https://oeis.org/A057722
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n^4 - 3*n^2 + 1
 * Range:     i8 n <= 3, i16 n <= 13, i32 n <= 215, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t x  = (seq_t)n;          /* x = n */
    seq_t x2 = x * x;             /* x2 = n^2 */
    seq_t x4 = x2 * x2;           /* x4 = n^4 */
    return x4 - (seq_t)3 * x2 + (seq_t)1;
}
