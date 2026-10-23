/*
 * A163822  Number of divisors d of 2n such that gcd(d-1,2n/d-1) = 1.
 * https://oeis.org/A163822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors d of 2n such that gcd(d-1, 2n/d-1) = 1
 * Range:     u8 n <= 1159, u16 n <= 9015, u32 n <= 9015, u64 n <= 9015,
 *            u128 n <= 9015, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    idx_t m = n * 2;
    seq_t count = 0;
    for (idx_t d = 1; d * d <= m; ++d) {
        if (m % d == 0) {
            idx_t e = m / d;
            seq_t d1 = (seq_t)d - 1;
            seq_t e1 = (seq_t)e - 1;
            seq_t g = gcd(d1, e1);
            if (g == 1) {
                ++count;
                if (d != e) ++count;
            }
        }
    }
    return count;
}
