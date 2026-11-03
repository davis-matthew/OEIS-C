/*
 * A116400  Expansion of e.g.f. Bessel_I(2,2x) + Bessel_I(3,2x) +
 *          Bessel_I(4,2x).
 * https://oeis.org/A116400
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=0, a(1)=0, for n≥2 a(n) = C( 2*⌊n/2⌋ + 1 , ⌊n/2⌋ – 1 )
 *            where C(m,k) is the binomial coefficient
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 35, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Validated: partial, pass, 37 terms through n = 36 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;
    idx_t k = n / 2;                       /* floor(n/2) */
    return binomial(2 * k + 1, k - 1);        /* C(2k+1, k-1) */
}
