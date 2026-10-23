/*
 * A133948  a(n) = the number of "isolated divisors" of n(n+1). A positive
 *          divisor, k, of n is isolated if neither (k-1) nor (k+1) divides n.
 * https://oeis.org/A133948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the number of divisors d of n(n+1) such that neither d-1
 *            nor d+1 divides n(n+1)
 * Range:     u8 n <= 2078, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_isqrt_u64.h"

seq_t a(idx_t n)
{
    /* m = n*(n+1) fits in 64 bits for n up to 10000 (and the tests). */
    uint64_t m = (uint64_t)n * ((uint64_t)n + 1);
    /* sqrt floor of m */
    idx_t limit = floor_isqrt_u64(m);

    seq_t isolated = 0;

    for (idx_t d = 1; d <= limit; ++d)
    {
        if (m % d != 0)
            continue;        /* d is not a divisor */

        /* divisor d itself */
        uint64_t d1 = d;
        bool iso1 = true;
        if (d1 > 1 && m % (d1 - 1) == 0) iso1 = false;
        if (iso1 && d1 < m && m % (d1 + 1) == 0) iso1 = false;
        if (iso1) isolated++;

        /* complementary divisor d2 = m/d1; distinct because m is not a square */
        uint64_t d2 = m / d1;
        bool iso2 = true;
        if (d2 > 1 && m % (d2 - 1) == 0) iso2 = false;
        if (iso2 && d2 < m && m % (d2 + 1) == 0) iso2 = false;
        if (iso2) isolated++;
    }

    return isolated;
}
