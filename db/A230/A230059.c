/*
 * A230059  Conjectural number of irreducible zeta values of weight 2*n+1 and
 *          depth three.
 * https://oeis.org/A230059
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 0 for n <= 4; otherwise floor(((n-1)^2-1)/12)
 * Range:     u8 n <= 17, u16 n <= 29, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= (idx_t)4)
        return (seq_t)0;
    /* n - 1 fits into idx_t since n >= 5 */
    seq_t nn = (seq_t)(n - (idx_t)1);   /* (n - 1) as seq_t */
    seq_t val = nn * nn;                /* (n-1)^2   */
    val -= (seq_t)1;                    /* (n-1)^2 - 1*/
    return val / (seq_t)12;            /* floor division by 12 */
}
