/*
 * A185149  a(n) = 3^n*A003046(n+1)/A002457(n).
 * https://oeis.org/A185149
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∏_{k=0}^{n-1} (3*(k+1)*Catalan(k+1))/(k+3)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   (Catalan(k) = A000108(k)) */
#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    seq_t result = (seq_t)1;
    for (idx_t k = 0; k < n; ++k)
    {
        seq_t cat   = catalan(k + 1);          /* A000108(k+1) */
        seq_t term  = cat;                     /* start with Catalan */
        term *= (seq_t)3 * (seq_t)(k + 1);     /* 3*(k+1)*cat */
        term /= (k + 3);                       /* divide by (k+3) */
        result *= term;                        /* multiply into product */
    }
    return result;
}
