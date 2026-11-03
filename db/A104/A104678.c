/*
 * A104678  a(n) = binomial(n+4,4) * binomial(n+9,4).
 * https://oeis.org/A104678
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(n+4,4) * binomial(n+9,4)
 * Range:     u8 n <= 0, u16 n <= 4, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Validated: partial, pass, 27 terms through n = 26 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <sys/types.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t c1 = binomial(n + 4, 4);   /* C(n+4,4) = (n+4)(n+3)(n+2)(n+1)/24   */
    seq_t c2 = binomial(n + 9, 4);   /* C(n+9,4) = (n+9)(n+8)(n+7)(n+6)/24   */
    return c1 * c2;
}
