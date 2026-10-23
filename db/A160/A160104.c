/*
 * A160104  Numerator of Hermite(n, 5/27).
 * https://oeis.org/A160104
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 27^n * Hermite(n, 5/27). Recurrence: a(0) = 1, a(1) = 10,
 *            for n ≥ 1: a_{n+1} = 10 * a_n - 1458 * n * a_{n-1}
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 10, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return 1;
    }
    if (n == 1) {
        return 10;
    }

    seq_t a_prev = 1;          /* a_{0}  */
    seq_t a_curr = 10;         /* a_{1}  */
    idx_t i = 1;               /* current n in the recurrence */

    for (; i < n; ++i) {
        /* term = 1458 * i * a_prev  */
        seq_t term = (seq_t)1458 * (seq_t)i * a_prev;
        seq_t a_next = (seq_t)10 * a_curr - term;
        a_prev = a_curr;
        a_curr = a_next;
    }

    return a_curr;
}
