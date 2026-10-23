/*
 * A166152  a(n) = (6^n+12*(-7)^n)/13.
 * https://oeis.org/A166152
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = (6^n + 12*(-7)^n)/13
 * Range:     i8 n <= 2, i16 n <= 5, i32 n <= 11, i64 n <= 22, i128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1, a(1) = -6, a(n) = 42*a(n-2) - a(n-1) */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)(-6);

    seq_t a_nm2 = (seq_t)1;   /* a(0) */
    seq_t a_nm1 = (seq_t)(-6);/* a(1) */
    seq_t a_n = 0;

    for (idx_t i = 2; i <= n; ++i) {
        a_n = (seq_t)42 * a_nm2 - a_nm1;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
