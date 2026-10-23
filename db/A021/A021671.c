/*
 * A021671  Decimal expansion of 1/667.
 * https://oeis.org/A021671
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = 0 if n=0 else floor(10 * (10^n mod 667) / 667) (decimal
 *            digits of 1/667)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   _pow10_mod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_pow10_mod.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    uint64_t r = _pow10_mod(n);            /* r = 10^n mod 667  */
    return (seq_t)((10ULL * r) / 667ULL);
}
