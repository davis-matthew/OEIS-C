/*
 * A202689  a(n) = (2n)! * (n+1)! / 2^(2n).
 * https://oeis.org/A202689
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2n)! * (n+1)! / 2^(2n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Using the recurrence a(0)=1 and a(n+1)=a(n)*((2n^3+7n^2+7n+2)/2)   */
seq_t
a(idx_t n)
{
    seq_t res = 1;
    for (idx_t i = 0; i < n; ++i)
    {
        /* term = i  (cast to seq_t is automatic) */
        seq_t term = (seq_t)i;
        /* compute (2*i^3 + 7*i^2 + 7*i + 2)/2 as an integer */
        seq_t f_half = term * term * term;                     /* i^3      */
        seq_t tmp     = 7 * term * term + 7 * term + 2;        /* 7i^2+7i+2*/
        f_half += tmp / 2;                                     /* divide by 2    */
        res *= f_half;                                         /* a(i+1) = a(i)*f_half  */
    }
    return res;
}
