/*
 * A182255  a(n) = 81*n^2 - 2247*n + 15383.
 * https://oeis.org/A182255
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 81*n^2 - 2247*n + 15383
 * Range:     i8 none, i16 n <= 34, i32 n <= 40, i64 n <= 40, i128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;                /* cast index to seq_t */
    seq_t term = 81 * nn * nn - 2247 * nn + 15383;
    return term;
}
