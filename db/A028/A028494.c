/*
 * A028494  a(n) = -(1/2)*(n+2)*(n^2 - 6*n - 1).
 * https://oeis.org/A028494
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -(1/2)*(n+2)*(n^2 - 6*n - 1)
 * Range:     i8 n <= 7, i16 n <= 33, i32 n <= 740, i64 n <= 740,
 *            i128 n <= 740, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Convert the index to seq_t first to avoid mixed‑type overflow */
    seq_t N = (seq_t)n;
    seq_t part1 = N + 2;                       /* n + 2   */
    seq_t part2 = N * N - 6 * N - 1;           /* n^2 - 6n - 1 */
    seq_t prod  = part1 * part2;               /* (n+2)*(n^2 - 6n - 1) */
    /* prod is always even, so division is exact */
    return -(prod / 2);                        /* -(1/2)*prod */
}
