/*
 * A143849  Let x(1) = 1 and x(n) = x(n-1)^2 + i for n>1; a(n) is the
 *          imaginary part of x(n).
 * https://oeis.org/A143849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1)=0; a(n)=Im(x(n)) where x(1)=1 and x(n)=x(n-1)^2 + i for n>1
 * Range:     i8 n <= 5, i16 n <= 6, i32 n <= 7, i64 n <= 8, i128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < (idx_t)1) return (seq_t)0;
    seq_t u = (seq_t)1;  /* real part of x(1) */
    seq_t v = (seq_t)0;  /* imaginary part of x(1) */
    for (idx_t i = (idx_t)2; i <= n; ++i)
    {
        seq_t u_next = u * u - v * v;
        seq_t v_next = (u * v) * (seq_t)2 + (seq_t)1;
        u = u_next;
        v = v_next;
    }
    return v;
}
