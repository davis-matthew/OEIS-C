/*
 * A192462  Constant term of the reduction by x^2->x+1 of the polynomial
 *          p(n,x) defined below in Comments.
 * https://oeis.org/A192462
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = constant term of the reduction of * p(n,x) where
 *            p(0,x)=1, p(n,x) = (n*x+1)*p(n-1,x), * reduction rule: x^2 ->
 *            x+1. * Recurrence: * a(0)=1, b(0)=0 * a(n) = a(n-1) + n*b(n-1) *
 *            b(n) = n*a(n-1) + (n+1)*b(n-1) (b is the coefficient of x) * The
 *            function returns a(n)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t a_val = 1;   /* a(0) */
    seq_t b_val = 0;   /* b(0) */

    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t new_a = a_val + (seq_t)i * b_val;
        seq_t new_b = (seq_t)i * a_val + ((seq_t)i + 1) * b_val;
        a_val = new_a;
        b_val = new_b;
    }

    return a_val;
}
