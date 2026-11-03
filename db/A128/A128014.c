/*
 * A128014  Central binomial coefficients C(2k,k) repeated.
 * https://oeis.org/A128014
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = C(2*⌊n/2⌋, ⌊n/2⌋) (central binomial coefficient of
 *            floor(n/2))
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Validated: partial, pass, 34 terms through n = 33 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t k = n >> 1;            /* k = floor(n/2) */
    return binomial(k << 1, k);     /* C(2k,k) */
}
