/*
 * A167541  a(n) = -(n - 4)*(n - 5)*(n - 12)/6.
 * https://oeis.org/A167541
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -(n - 4)*(n - 5)*(n - 12)/6
 * Range:     i8 n <= 16, i16 n <= 48, i32 n <= 48, i64 n <= 48, i128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 6) return 0;
    seq_t nn = (seq_t)n;
    seq_t val = -((nn - 4) * (nn - 5) * (nn - 12) / 6);
    return val;
}
