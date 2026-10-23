/*
 * A107394  C(n+2,2)*C(n+4,2).
 * https://oeis.org/A107394
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = C(n+2,2) * C(n+4,2)
 * Range:     i8 n <= 2, i16 n <= 9, i32 n <= 9, i64 n <= 9, i128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    /* C(n+2,2) = (n+2)*(n+1)/2 */
    seq_t x = ((nn + 2) * (nn + 1)) / 2;
    /* C(n+4,2) = (n+4)*(n+3)/2 */
    seq_t y = ((nn + 4) * (nn + 3)) / 2;
    return x * y;
}
