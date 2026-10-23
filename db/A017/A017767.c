/*
 * A017767  Binomial coefficients C(51,n).
 * https://oeis.org/A017767
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = binomial(51,n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* For n > 51 the binomial coefficient is zero. */
    if (n > 51)
        return 0;
    return binomial(51, n);
}
