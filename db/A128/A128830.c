/*
 * A128830  a(n) = the number of positive divisors of n which are coprime to
 *          d(n), where d(n) = the number of positive divisors of n.
 * https://oeis.org/A128830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{d | d divides n, gcd(d, d(n)) = 1}|, where d(n) is the
 *            number of positive divisors of n
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

static idx_t floor_isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n, mid, res = 0;
    while (lo <= hi) {
        mid = lo + ((hi - lo) >> 1);
        __int128 prod = (__int128)mid * mid;
        if (prod <= n) {
            res = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0, but safe */

    /* Count total number of divisors d(n). */
    seq_t tau = 0;
    idx_t s = floor_isqrt_u64(n);
    for (idx_t d = 1; d <= s; ++d) {
        if (n % d == 0) {
            ++tau;
            if (d != n / d) ++tau;
        }
    }

    /* Count those divisors coprime to tau. */
    seq_t coprime = 0;
    for (idx_t d = 1; d <= s; ++d) {
        if (n % d == 0) {
            idx_t e = n / d;
            if (gcd((seq_t)d, tau) == 1) ++coprime;
            if (e != d && gcd((seq_t)e, tau) == 1) ++coprime;
        }
    }
    return coprime;
}
