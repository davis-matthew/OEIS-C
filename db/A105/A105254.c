/*
 * A105254  a(n) = binomial(n+7,n)*binomial(n+11,n).
 * https://oeis.org/A105254
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(n+7,n) * binomial(n+11,n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 44, u128 n <= 581,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* compute C(n+7, n) = C(n+7, 7) and C(n+11, n) = C(n+11, 11) */
    seq_t term1 = binomial(n + 7, 7);   /* C(n+7,7) */
    seq_t term2 = binomial(n + 11, 11); /* C(n+11,11) */

    return term1 * term2;
}
