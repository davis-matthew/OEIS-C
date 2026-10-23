/*
 * A027788  a(n) = 15*(n+1)*binomial(n+2,15)/2.
 * https://oeis.org/A027788
 *
 * Offset:    13    (a(13) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(n+1,14) * C(n+2,2)
 * Range:     u8 n <= 13, u16 n <= 15, u32 n <= 25, u64 n <= 85,
 *            u128 n <= 1000, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Defined for n >= 13; return 0 otherwise. */
    if (n < 13)
        return 0;

    /* Binomial coefficients provided by the utils library. */
    seq_t f1 = binomial(n + 1, 14);  // C(n+1,14)
    seq_t f2 = binomial(n + 2, 2);   // C(n+2,2)

    /* Multiply the two factors.  The product is computed in seq_t
       and wraps correctly if it does not fit, as required. */
    return f1 * f2;
}
