/*
 * A152689  Apply partial sum operator thrice to factorials.
 * https://oeis.org/A152689
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=0..n-1} ((j-1)*!j - j! + 1), where !j =
 *            sum_{k=0}^{j-1} k! (with !0 = 0, 0! = 1)
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 15, u64 n <= 23, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;          /* a(n) */
    seq_t fact   = 1;          /* j!  (starts at 0! ) */
    seq_t sf     = 0;          /* !j  (starts at !0 = 0) */

    for (idx_t j = 0; j < n; ++j)
    {
        if (j >= 2)           /* terms for j = 0,1 are zero */
        {
            seq_t term = (seq_t)(j - 1) * sf; /* (j-1)*!j */
            term -= fact;                    /* - j!    */
            term += 1;                       /* + 1     */
            result += term;
        }

        /* Prepare values for the next iteration (j+1) */
        sf += fact;                /* sf becomes !(j+1) */
        fact *= (j + 1);           /* fact becomes (j+1)! */
    }

    return result;
}
