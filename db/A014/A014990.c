/*
 * A014990  a(n) = (1 - (-8)^n)/9.
 * https://oeis.org/A014990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (1 - (-8)^n)/9 * Recurrence: a(1) = 1, a(2) = -7, * a(n)
 *            = -7*a(n-1) + 8*a(n-2) (n > 2)
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 11, i64 n <= 22, i128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) {
        return (seq_t)1;
    }
    if (n == 2) {
        return (seq_t)-7;
    }

    seq_t a_nm2 = (seq_t)1;   /* a(n-2) starting with a(1) */
    seq_t a_nm1 = (seq_t)-7;  /* a(n-1) starting with a(2) */
    seq_t a_n;

    for (idx_t i = 3; i <= n; ++i) {
        a_n = (seq_t)-7 * a_nm1 + (seq_t)8 * a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
