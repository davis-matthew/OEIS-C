/*
 * A087670  The first integer in the sequence defined by b(0)=(2n+1)/n and
 *          b(k+1)=b(k)*floor(b(k)) for k>=0.
 * https://oeis.org/A087670
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the first integer in the sequence defined by b(0) =
 *            (2n+1)/n and b(k+1) = b(k) * ⌊b(k)⌋ for k ≥ 0. The function
 *            returns that integer. (The starting value always lies in (2,3]
 *            for n>1, and the recurrence quickly inflates the numerator until
 *            it becomes a multiple of n.)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 4, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* start with numerators of b(0) = (2n+1)/n;
       denominator is always n (fixed)       */
    seq_t a_num = ((seq_t)n) * 2 + 1;   /* numerator of current b */
    seq_t denom  = (seq_t)n;            /* constant denominator   */

    /* Iterate until n divides the numerator, i.e. b(k) is integer. */
    while (a_num % denom != 0)
    {
        seq_t floor_val = a_num / denom;  /* ⌊b(k)⌋ */
        a_num = a_num * floor_val;        /* b(k+1) numerator  */
    }

    /* b(k) = a_num / denom is now integer; return it. */
    return a_num / denom;
}
