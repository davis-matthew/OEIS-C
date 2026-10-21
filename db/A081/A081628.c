/*
 * A081628  a(n) = 2*(-1)^n - (-5)^n.
 * https://oeis.org/A081628
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 2*(-1)^n - (-5)^n
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 13, i64 n <= 27, i128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=3, a(n) = -6*a(n-1) - 5*a(n-2) for n>=2 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;

    seq_t a_nm2 = (seq_t)1;   /* a(n-2) */
    seq_t a_nm1 = (seq_t)3;   /* a(n-1) */
    seq_t a_n   = 0;

    for (idx_t i = 2; i <= n; ++i) {
        a_n   = - (seq_t)6 * a_nm1 - (seq_t)5 * a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
