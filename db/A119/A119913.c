/*
 * A119913  Number of directed simple cycles in the complete graph K_n.
 * https://oeis.org/A119913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=3..n} C(n,k) * (k-1)!
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h> /* for uint64_t, but type idx_t is provided by build */

seq_t a(idx_t n)
{
    if (n < 3)
        return (seq_t)0;          /* a(1)=0, a(2)=0 */

    seq_t result = 0;
    seq_t binom  = 1;             /* binom = C(n,0) initially */
    seq_t fact   = 1;             /* fact   = (0)! initially */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* binom = C(n,k) */
        binom = (binom * (seq_t)(n - k + 1)) / (seq_t)k;

        if (k >= 3)
            result += binom * fact;   /* fact = (k-1)! at this point */

        /* prepare fact for next iteration: fact = k! */
        fact *= (seq_t)k;
    }
    return result;
}
