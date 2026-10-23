/*
 * A017766  Binomial coefficients C(50,n).
 * https://oeis.org/A017766
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = binomial(50, n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
seq_t a(idx_t n)
{
    if (n > 50)
        return (seq_t)0;
    return binomial(50, n);
}
