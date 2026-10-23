/*
 * A194523  Second coordinate of (4,5)-Lagrange pair for n.
 * https://oeis.org/A194523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n - 4*floor((n + 2)/9) - 2*floor((n + 5)/9) - 2*floor((n
 *            + 6)/9)
 * Range:     i8 n <= 96, i16 n <= 96, i32 n <= 96, i64 n <= 96, i128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* OEIS offset is 1, but for safety return 0 for n == 0 */
    if (n == 0) return 0;

    /* cast to seq_t so that arithmetic fits the return type */
    seq_t nn = (seq_t)n;
    seq_t t1 = (seq_t)(n + 2) / 9;   /* floor((n+2)/9) */
    seq_t t2 = (seq_t)(n + 5) / 9;   /* floor((n+5)/9) */
    seq_t t3 = (seq_t)(n + 6) / 9;   /* floor((n+6)/9) */

    return nn - 4 * t1 - 2 * t2 - 2 * t3;
}
