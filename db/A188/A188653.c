/*
 * A188653  Second differences of A000463; first differences of A188652.
 * https://oeis.org/A188653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = ((2*n^2-5)*(-1)^n+1)/4
 * Range:     i8 n <= 8, i16 n <= 255, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t sign = (n % 2 == 0) ? 1 : -1;            /* (-1)^n */
    seq_t n_sq = (seq_t)n * (seq_t)n;             /* n^2 */
    seq_t part = ((seq_t)2 * n_sq - 5);            /* 2*n^2 - 5 */
    seq_t num  = part * sign + 1;                 /* ((2*n^2-5)*(-1)^n + 1) */
    return num / 4;                               /* exact division by 4 */
}
