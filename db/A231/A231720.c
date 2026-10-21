/*
 * A231720  a(0)=1, after which, for any n uniquely written as du*u! + ... +
 *          d2*2! + d1*1! (each di in range 0..i), a(n) = (du+1)*(u+1)! + ...
 *          + (d2+1)*3! + (d1+1)*2! + 1; the natural numbers with their
 *          factorial base representation (A007623) shifted left one step and
 *          each digit incremented by one, converted...
 * https://oeis.org/A231720
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 1 + Σ_{i≥1} (d_i+1)*(i+1)! where d_i are the factorial
 *            digits of n (n = Σ_{i≥1} d_i * i! )
 * Range:     u8 n <= 23, u16 n <= 5039, u32 n <= 9627, u64 n <= 9627,
 *            u128 n <= 9627, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t  result = 1;          /* the constant +1 term */
    idx_t  i      = 1;          /* current digit position (1-based) */
    idx_t  tmp    = n;          /* working copy of the index */
    idx_t  fact   = 1;           /* will hold (i+1)! during the loop */

    while (tmp > 0)
    {
        fact *= (i + 1);                   /* fact = (i+1)! */
        idx_t digit = tmp % (i + 1);        /* d_i  */
        result += (seq_t)(digit + 1) * (seq_t)fact;
        tmp /= (i + 1);
        ++i;
    }

    return result;
}
