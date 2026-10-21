/*
 * A215695  a(n) = 5*a(n-1) - 6*a(n-2) + a(n-3) with a(0)=1, a(1)=0, a(2)=-2.
 * https://oeis.org/A215695
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 5*a(n-1) - 6*a(n-2) + a(n-3), a(0)=1, a(1)=0, a(2)=-2
 * Range:     i8 n <= 5, i16 n <= 9, i32 n <= 19, i64 n <= 38, i128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)-2;

    seq_t a_nm3 = (seq_t)1;   /* a(n-3) */
    seq_t a_nm2 = (seq_t)0;   /* a(n-2) */
    seq_t a_nm1 = (seq_t)-2;  /* a(n-1) */
    seq_t a_n;

    for (idx_t i = 3; i <= n; ++i) {
        a_n = (seq_t)5 * a_nm1 - (seq_t)6 * a_nm2 + a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_nm1;
}
