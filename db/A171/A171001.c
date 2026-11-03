/*
 * A171001  Binomial(n-k,k)^2 where k = ceiling(n/4).
 * https://oeis.org/A171001
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial( n - ceil(n/4) , ceil(n/4) )^2
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 26, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Validated: partial, pass, 32 terms through n = 31 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* k = ceil(n/4) */
    idx_t k = (n + 3) / 4U;          /* unsigned arithmetic: ceil(n/4) */
    /* compute the binomial coefficient C(n-k, k) */
    seq_t c = binomial(n - k, k);
    /* return its square */
    return c * c;
}
