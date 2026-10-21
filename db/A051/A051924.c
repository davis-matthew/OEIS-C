/*
 * A051924  a(n) = binomial(2*n,n) - binomial(2*n-2,n-1); or (3n-2)*C(n-1),
 *          where C = Catalan numbers (A000108).
 * https://oeis.org/A051924
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = (3*n-2) * Catalan(n-1) (offset 1)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 66,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    /* a(1) = 1  */
    seq_t c     = catalan(n-1);           /* Catalan number C(n‑1)          */
    seq_t factor = 3*(seq_t)n - 2;        /* (3*n-2) as seq_t               */
    return c * factor;                    /* product gives a(n)             */
}
