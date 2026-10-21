/*
 * A051764  Number of torus knots with n crossings.
 * https://oeis.org/A051764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = |{k: sqrt(n) < k <= n, k|n, gcd(k, 1 + n/k) = 1}|
 *            Computes the number of torus knots with n crossings
 * Range:     u8 n <= 613, u16 n <= 9584, u32 n <= 9584, u64 n <= 9584,
 *            u128 n <= 9584, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

/* Integer square root: floor(sqrt(n)) for 0 <= n <= 2^64-1 */
static idx_t isqrt_u64(idx_t n)
{
    idx_t lo = 0;
    idx_t hi = (idx_t)1 << 32;          /* hi^2 > 2^64-1 for all cases */
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if ((unsigned __int128)mid * mid > n)
            hi = mid - 1;
        else
            lo = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    seq_t count = 0;
    idx_t lim = isqrt_u64(n);          /* floor(sqrt(n)) */

    for (idx_t i = 1; i <= lim; ++i) {
        if (n % i == 0) {
            idx_t d = n / i;           /* larger divisor, d >= i */
            if (d > lim) {             /* ensure sqrt(n) < k <= n */
                if (gcd((seq_t)d, (seq_t)(1 + i)) == 1)
                    ++count;
            }
        }
    }
    return count;
}
