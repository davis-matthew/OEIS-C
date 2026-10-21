/*
 * A091551  Second column (k=3) sequence of array ((7,2)-Stirling2) divided by
 *          14.
 * https://oeis.org/A091551
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * a(n) = (∏_{j=0..n-1} (5*j+2)) * * ( -3 * ∏_{j=0..n-1} (5*j+1)
 *            + ∏_{j=0..n-1} (5*j+3) ) / (3!*14) * Offset: a(2) is the first
 *            term of the OEIS sequence
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 2)   /* values before the offset are irrelevant */
        return (seq_t)0;

    /* For n = 2 the products are easily pre‐calculated: */
    seq_t S = (seq_t)14;   /* ∏_{j=0}^{1} (5*j+2) = 2·7               */
    seq_t L = (seq_t)6;    /* ∏_{j=0}^{1} (5*j+1) = 1·6               */
    seq_t D = (seq_t)6;    /* M - 3L for n = 2, where M = ∏(5*j+3)  */

    if (n > 2) {
        for (idx_t i = 3; i <= n; ++i) {
            /* D_i = (5*i-2)*D_{i-1} + 6*L_{i-1}   (derived from the product formula) */
            seq_t D_new = ((seq_t)(5*i - 2)) * D + ((seq_t)6) * L;

            /* Update the products for the next index: */
            seq_t multS = (seq_t)(5*(i-1) + 2);   /* (5*(i-1)+2)  = (5j+2) for j = i-1 */
            seq_t multL = (seq_t)(5*(i-1) + 1);   /* (5*(i-1)+1)  = (5j+1) for j = i-1 */

            S *= multS;   /* ∏_{j=0}^{i-1} (5*j+2)  after the update   */
            L *= multL;   /* ∏_{j=0}^{i-1} (5*j+1)  after the update   */
            D  = D_new;   /* prepare D_i for the next round          */
        }
    }

    /* Final division by 84 (3!*14).  The division is exact by construction. */
    return S * D / 84;
}
