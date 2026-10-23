/*
 * A178671  a(n) = 5^n - 5.
 * https://oeis.org/A178671
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 5*a(n-1) + 20 with a(0) = -4 (equivalent to 5^n - 5)
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 13, i64 n <= 27, i128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t res = (seq_t)-4;
    for (idx_t i = 1; i <= n; ++i) {
        res = 5 * res + 20;
    }
    return res;
}
