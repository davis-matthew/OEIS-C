/*
 * A120781  Denominators of partial sums of Catalan numbers scaled by powers
 *          of 1/8.
 * https://oeis.org/A120781
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n)=denominator(∑_{k=0}^n Catalan(k)/8^k) is a power of 2. Let
 *            e(0)=0. For n≥1: if n is even : e(n)=e(n-1)+2 if n is odd :
 *            e(n)=e(n-1)+3+v2((n+1)/2) where v2(m) is the exponent of 2
 *            dividing m (number of trailing zeros). Then a(n)=2^e(n)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    idx_t e = 0;               /* exponent of 2 in the denominator */

    for (idx_t i = 1; i <= n; ++i) {
        if ((i & 1ULL) == 0ULL) {            /* even i */
            e += 2ULL;
        } else {                              /* odd i */
            idx_t t = (i + 1ULL) >> 1ULL;    /* (i+1)/2  ≥ 1 */
            /* compute v2(t) – trailing zeros of t */
            idx_t tz = 0ULL;
            while ((t & 1ULL) == 0ULL) {
                t >>= 1ULL;
                ++tz;
            }
            e += 3ULL + tz;
        }
    }

    return pow2(e);          /* pow2(exp) returns 2^exp as seq_t   */
}
