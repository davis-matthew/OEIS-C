/*
 * A159485  Numerator of Hermite(n, 7/12).
 * https://oeis.org/A159485
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 7*a(n-1) - 72*(n-1)*a(n-2), a(0)=1, a(1)=7
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 7, i64 n <= 15, i128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)1;
    if (n == (idx_t)1)
        return (seq_t)7;

    seq_t a_nm2 = (seq_t)1;  /* a(0) */
    seq_t a_nm1 = (seq_t)7;  /* a(1) */
    seq_t a_n;

    for (idx_t i = (idx_t)2; i <= n; ++i)
    {
        seq_t coeff = (seq_t)72 * (seq_t)(i - (idx_t)1);
        a_n = (seq_t)7 * a_nm1 - coeff * a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_nm1;
}
