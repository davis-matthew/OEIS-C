/*
 * A125082  a(n) = n^4 - n^3 - n^2 - n - 1.
 * https://oeis.org/A125082
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n^4 - n^3 - n^2 - n - 1
 * Range:     i8 n <= 3, i16 n <= 13, i32 n <= 215, i64 n <= 660,
 *            i128 n <= 660, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t x = (seq_t)n;
    seq_t x2 = x * x;        /* n^2 */
    seq_t x3 = x2 * x;       /* n^3 */
    seq_t x4 = x3 * x;       /* n^4 */
    return x4 - x3 - x2 - x - 1;
}
