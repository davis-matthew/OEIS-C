/*
 * A193041  Coefficient of x in the reduction by x^2->x+1 of the polynomial
 *          p(n,x) defined at Comments.
 * https://oeis.org/A193041
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   a(0)=0; let b(0)=1. For n>=1:
   a(n)=a(n-1)+b(n-1);
   b(n)=a(n-1)+1+n^3.
   Return a(n) which is the coefficient of x in the reduced polynomial p(n,x).
 */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    seq_t a_val = (seq_t)0;
    seq_t b_val = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t ii = (seq_t)i;
        seq_t cube = ii * ii * ii;
        seq_t new_a = a_val + b_val;
        seq_t new_b = a_val + (seq_t)1 + cube;
        a_val = new_a;
        b_val = new_b;
    }
    return a_val;
}
