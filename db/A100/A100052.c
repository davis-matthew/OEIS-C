/*
 * A100052  A Chebyshev transform of the odd numbers.
 * https://oeis.org/A100052
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(0)=1, a(1)=3, a(2)=3, a(3)=-2;
 *            a(n)=2*a(n-1)-3*a(n-2)+2*a(n-3)-a(n-4) for n>=5 (the
 *            inhomogeneous term g_n has value -1 at n=4, and 0 thereafter)
 * Range:     i8 n <= 59, i16 n <= 59, i32 n <= 59, i64 n <= 59, i128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)(-2);

    seq_t a_nm4 = (seq_t)1;         /* a(n-4) */
    seq_t a_nm3 = (seq_t)3;         /* a(n-3) */
    seq_t a_nm2 = (seq_t)3;         /* a(n-2) */
    seq_t a_nm1 = (seq_t)(-2);      /* a(n-1) */
    seq_t an = 0;

    for (idx_t i = 4; i <= n; ++i) {
        if (i == 4) {
            /* g_4 = -1 */
            an = -((seq_t)1) + 2*a_nm1 - 3*a_nm2 + 2*a_nm3 - a_nm4;
        } else {
            /* homogeneous recurrence for n>=5 */
            an = 2*a_nm1 - 3*a_nm2 + 2*a_nm3 - a_nm4;
        }
        a_nm4 = a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = an;
    }
    return an;
}
