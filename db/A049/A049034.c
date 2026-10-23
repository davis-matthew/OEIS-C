/*
 * A049034  Scaled sums of odd reciprocals.
 * https://oeis.org/A049034
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n+1)! * Sum_{k=0..n} 1/(2*k+1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;
    seq_t f      = 1;   /* factorial of 1 = (2*0+1)!          */
    seq_t a_val  = 1;   /* a(0)                               */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t two   = (seq_t)2 * i;                 /* 2*i                     */
        seq_t a_prev = a_val;                       /* a(i-1)                  */
        seq_t f_prev = f;                           /* (2*i-1)!                */
        f = f_prev * two * (two + (seq_t)1);        /* (2*i+1)! = f_prev*2i*(2i+1) */
        a_val = two * ( (two + (seq_t)1) * a_prev + f_prev );
    }
    return a_val;
}
