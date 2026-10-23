/*
 * A033931  a(n) = lcm(n,n+1,n+2).
 * https://oeis.org/A033931
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = lcm(n,n+1,n+2)
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1624, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* The OEIS offset is 1, so n >= 1.  For safety we return 0 when
       n == 0, although the caller should not request it. */
    if (n == 0)
        return 0;

    seq_t x = (seq_t)n;
    seq_t y = x + 1;
    seq_t z = x + 2;

    seq_t g1 = gcd(x, y);
    seq_t l1 = (x / g1) * y;

    seq_t g2 = gcd(l1, z);
    seq_t result = (l1 / g2) * z;

    return result;
}
