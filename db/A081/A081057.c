/*
 * A081057  E.g.f.: Sum_{n>=0} a(n)*x^n/n! = {Sum_{n>=0} F(n+1)*x^n/n!}^2,
 *          where F(n) is the n-th Fibonacci number.
 * https://oeis.org/A081057
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 2*a(n-2) - 4*a(n-3) Initial values: a(0)=1,
 *            a(1)=2, a(2)=6
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)6;

    seq_t a_nm3 = (seq_t)1;   /* a(n-3)  */
    seq_t a_nm2 = (seq_t)2;   /* a(n-2)  */
    seq_t a_nm1 = (seq_t)6;   /* a(n-1)  */
    seq_t a_cur;

    for (idx_t i = 3; i <= n; ++i)
    {
        a_cur = a_nm1 * (seq_t)3 + a_nm2 * (seq_t)2 - a_nm3 * (seq_t)4;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_cur;
    }
    return a_nm1;
}
