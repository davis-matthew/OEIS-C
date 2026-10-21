/*
 * A228537  x-values in the solution to the Pell equation x^2 - 58*y^2 = -1.
 * https://oeis.org/A228537
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 39206*a(n-1) - a(n-2), a(1)=99, a(2)=3881493
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* offset 1; n=0 not used */
    if (n == 1) return (seq_t)99;
    if (n == 2) return (seq_t)3881493;

    seq_t a_nm2 = (seq_t)99;          /* a(n-2) */
    seq_t a_nm1 = (seq_t)3881493;     /* a(n-1) */
    seq_t a_n;                       /* a(n) */

    for (idx_t i = 3; i <= n; ++i)
    {
        a_n = (seq_t)39206 * a_nm1 - a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_nm1;
}
