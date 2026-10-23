/*
 * A159564  Numerator of Hermite(n, 1/19).
 * https://oeis.org/A159564
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 19^n * Hermite(n, 1/19)
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 5, i64 n <= 11, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    seq_t a_prev = (seq_t)1; /* a(0) */
    seq_t a_curr = (seq_t)2; /* a(1) */

    for (idx_t i = 2; i <= n; ++i) {
        /* term = 722 * (i-1) */
        seq_t term = (seq_t)722 * (seq_t)(i - 1);
        /* a(i) = 2*a(i-1) - term * a(i-2) */
        seq_t a_next = 2 * a_curr - term * a_prev;
        a_prev = a_curr;
        a_curr = a_next;
    }
    return a_curr;
}
