/*
 * A192755  Coefficient of x in the reduction by x^2->x+1 of the polynomial
 *          p(n,x) defined below in Comments.
 * https://oeis.org/A192755
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(1)=1, a(2)=7, a(3)=19; For n>=4:
 *            a(n)=a(n-4)-a(n-3)-2*a(n-2)+3*a(n-1) (derived from the
 *            generating function -x*(1+4*x)/((x^2+x-1)*(x-1)^2) )
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)7;
    if (n == 3) return (seq_t)19;

    seq_t a0 = (seq_t)0;   /* a(n-4)   */
    seq_t a1 = (seq_t)1;   /* a(n-3)   */
    seq_t a2 = (seq_t)7;   /* a(n-2)   */
    seq_t a3 = (seq_t)19;  /* a(n-1)   */
    seq_t a4;

    for (idx_t i = 4; i <= n; ++i) {
        /* compute a4 = a0 + 3*a3 - (a1 + 2*a2) ;
           the subtraction is safe because a0 + 3*a3 >= a1 + 2*a2   */
        seq_t temp1 = a0 + (seq_t)3 * a3;
        seq_t temp2 = a1 + (seq_t)2 * a2;
        a4        = temp1 - temp2;

        /* shift the window for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
    }
    return a4;
}
