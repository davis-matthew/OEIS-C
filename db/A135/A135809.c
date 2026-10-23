/*
 * A135809  Number of coincidence-free length n lists of 3-tuples with all
 *          numbers 1,...,n in tuple position k, for k=1,2,3.
 * https://oeis.org/A135809
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{j=0..n} ((-1)^(n-j)) * binomial(n,j) * (j!)^3
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    seq_t binom = 1;          /* C(n,0) = 1 */
    seq_t fac   = 1;          /* j!   initially 0! = 1 */
    seq_t pos   = 0;          /* sum of positive terms   */
    seq_t neg   = 0;          /* sum of negative terms   */

    for (idx_t j = 0; j <= n; ++j)
    {
        /* Update factorial: fac = j!  */
        if (j > 0)
            fac *= (seq_t)j;

        /* term = C(n,j) * (j!)^3  */
        seq_t term_fac3 = fac;
        term_fac3 *= fac;
        term_fac3 *= fac;
        seq_t term = binom * term_fac3;

        /* add to positive or negative accumulator  */
        if ( ((n - j) & 1) == 0 )
            pos += term;
        else
            neg += term;

        /* prepare binom for next j   */
        if (j < n)
            binom = binom * (seq_t)(n - j) / (seq_t)(j + 1);
    }

    /* final result: pos >= neg  */
    return pos - neg;
}
