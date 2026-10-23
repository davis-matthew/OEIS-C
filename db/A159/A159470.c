/*
 * A159470  Numerator of Hermite(n, 10/11).
 * https://oeis.org/A159470
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 20*a(n-1) - 242*(n-1)*a(n-2), a(0)=1, a(1)=20
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 6, i64 n <= 12, i128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)20;

    seq_t a_nm2 = (seq_t)1;          /* a(0) */
    seq_t a_nm1 = (seq_t)20;         /* a(1) */
    seq_t a_n = 0;

    for (idx_t i = 2; i <= n; ++i) {
        /* term1 = 20 * a(n-1) */
        seq_t term1 = a_nm1 * (seq_t)20;
        /* term2 = 242 * (i-1) * a(n-2) */
        seq_t term2 = a_nm2 * (seq_t)242 * (seq_t)(i - 1);
        /* a(n) = term1 - term2 */
        a_n = term1 - term2;
        /* shift for next iteration */
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
