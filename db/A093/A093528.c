/*
 * A093528  Numerators of odd raw moments in the distribution of line lengths
 *          for lines picked at random in the unit disk.
 * https://oeis.org/A093528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = 2^exp(n) where exp(n) = 4n + 7 - popcount(n+1) -
 *            popcount(n+2) - 2*v2(n+1) - v2(n+2) + popcount(2n) (v2(x) =
 *            exponent of 2 in x)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   pow2, tz_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow2.h"
#include "../utils/tz_u64.h"

static inline idx_t popcount_u64(idx_t x)
{
    idx_t c = 0;
    while (x) {
        x &= x - 1;
        ++c;
    }
    return c;
}

seq_t a(idx_t n)
{
    /* n >= 1 */
    idx_t k = n;
    idx_t exp = 4 * k + 7;
    exp -= popcount_u64(k + 1) + popcount_u64(k + 2);
    exp -= 2 * tz_u64(k + 1);
    exp -= tz_u64(k + 2);
    exp += popcount_u64(2 * k);
    return pow2(exp);
}
