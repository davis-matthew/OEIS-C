/*
 * A072505  a(n) = n / (LCM of divisors of n which are <= sqrt(n)).
 * https://oeis.org/A072505
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = n / LCM{ d | d divides n, d <= sqrt(n) }
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

static inline idx_t int_sqrt(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (mid > 0 && mid > n / mid) {
            hi = mid - 1;
        } else {
            if ((mid + 1) > 0 && (mid + 1) > n / (mid + 1))
                return mid;
            lo = mid + 1;
        }
    }
    return hi; /* unreachable */
}

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t lcm = 1;
    idx_t sqrt_n = int_sqrt(n);

    for (idx_t d = 1; d <= sqrt_n; ++d) {
        if (n % d == 0) {
            seq_t sd = (seq_t)d;
            seq_t g = gcd(lcm, sd);
            lcm = lcm / g * sd;
        }
    }
    return N / lcm;
}
