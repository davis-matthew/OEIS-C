/*
 * A069949  a(n) = Sum_{d|n} phi(d+1).
 * https://oeis.org/A069949
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} phi(d+1)
 * Range:     u8 n <= 119, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _isqrt_idx, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/phi.h"
#include "../utils/_isqrt_idx.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t sq = _isqrt_idx(n);
    for (idx_t d = 1; d <= sq; ++d) {
        if (n % d == 0) {
            sum += phi(d + 1);
            idx_t d2 = n / d;
            if (d2 != d) {
                sum += phi(d2 + 1);
            }
        }
    }
    return sum;
}
