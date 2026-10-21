/*
 * A201555  a(n) = C(2*n^2,n^2) = A000984(n^2), where A000984 is the central
 *          binomial coefficients.
 * https://oeis.org/A201555
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = C(2*n^2, n^2) = (2*n^2)! / (n^2)!^2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"   /* provides seq_t binomial(idx_t n, idx_t k); */

seq_t a(idx_t n)
{
    /* n is an unsigned 64-bit index. For n=0 the formula yields 1. */
    idx_t k = n * n;         /* k = n^2, safe for n up to ~3·10^9. */
    return binomial(2 * k, k);
}
