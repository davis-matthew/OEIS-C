/*
 * A015242  a(n) = (2*n - 7)*n^2.
 * https://oeis.org/A015242
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (2*n - 7)*n^2
 * Range:     i8 n <= 5, i16 n <= 26, i32 n <= 1025, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t sn = (seq_t)n;
    seq_t sq = sn * sn;                /* n^2 */
    seq_t coeff = (sn * (seq_t)2) - (seq_t)7;  /* 2*n - 7 */
    return sq * coeff;                 /* (2*n - 7)*n^2 */
}
