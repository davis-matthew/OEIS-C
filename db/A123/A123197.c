/*
 * A123197  a(n) = (2*n+1)*(n+1)*(2*n^2+3*n-1).
 * https://oeis.org/A123197
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (2*n+1)*(n+1)*(2*n^2+3*n-1)
 * Range:     i8 n <= 1, i16 n <= 8, i32 n <= 151, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t x  = (seq_t)n;
    seq_t t1 = 2 * x + 1;
    seq_t t2 = x + 1;
    seq_t t3 = 2 * x * x + 3 * x - 1;
    return t1 * t2 * t3;
}
