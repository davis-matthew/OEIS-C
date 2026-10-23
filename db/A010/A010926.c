/*
 * A010926  Binomial coefficients C(10,n).
 * https://oeis.org/A010926
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = binomial(10, n)
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Binomial chooser from utils returns 0 for k > n, but guard for clarity */
    if (n > 10)
        return (seq_t)0;
    return binomial(10, n);
}
