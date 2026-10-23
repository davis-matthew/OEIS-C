/*
 * A169901  Earliest sequence such that (x+y) | a(xy) for all x>=1, y>=1.
 * https://oeis.org/A169901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = lcm_{x*y = n} (x + y)
 * Range:     u8 n <= 11, u16 n <= 29, u32 n <= 119, u64 n <= 629,
 *            u128 n <= 2519, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* minimal positive integer such that (x + y) | a(x*y) for all x,y >= 1 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t result = 1;
    for (idx_t d = 1; d*d <= n; ++d) {
        if (n % d == 0) {
            idx_t d2 = n / d;
            seq_t sum = (seq_t)d + (seq_t)d2;
            seq_t g = gcd(result, sum);
            result = (result / g) * sum;
        }
    }
    return result;
}
