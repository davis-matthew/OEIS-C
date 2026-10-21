/*
 * A067754  Number of unordered primitive solutions to xy+xz+yz=n.
 * https://oeis.org/A067754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of unordered primitive solutions to xy + xz + yz =
 *            n
 * Range:     u8 n <= 3049, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

static inline idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0;
    idx_t high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid != 0 && mid > n / mid) {
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    return high;
}

seq_t a(idx_t n)
{
    seq_t count = 0;
    idx_t max_x = isqrt_u64(n);
    for (idx_t x = 0; x <= max_x; ++x) {
        idx_t xs = x;
        idx_t m = n + xs * xs;
        idx_t sqrt_m = isqrt_u64(m);
        for (idx_t d = 1; d <= sqrt_m; ++d) {
            if (m % d) continue;
            idx_t e = m / d;
            if (d < 2 * xs) continue;
            seq_t xs_seq = (seq_t)xs;
            seq_t ys = (seq_t)d - xs_seq;
            seq_t zs = (seq_t)e - xs_seq;
            seq_t g = gcd(gcd(xs_seq, ys), zs);
            if (g == (seq_t)1) ++count;
        }
    }
    return count;
}
