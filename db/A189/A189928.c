/*
 * A189928  c(n) = n + [n*r/t] + [n*s/t]; r=1, s=sin(Pi/5), t=cos(Pi/5), where
 *          [] denotes the floor function.
 * https://oeis.org/A189928
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   c(n) = n + floor(n*sec(pi/5)) + floor(n*tan(pi/5))
 * Range:     u8 n <= 86, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square root: floor(sqrt(x)) for 0 ≤ x < 2^63 */
static inline uint64_t isqrt_uint64(uint64_t x)
{
    uint64_t lo = 0, hi = 65536; /* √(5·10⁸) ≈ 22361 < 2⁵⁶, so 65536 is a safe upper bound */
    while (lo < hi) {
        uint64_t mid = (lo + hi + 1) >> 1;
        if (mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* floor(n*sqrt(5)) = floor(sqrt(5*n*n)) */
    uint64_t nn = (uint64_t)n;
    uint64_t sq5 = 5 * nn * nn;          /* safe for n ≤ 10⁴ (max 5·10⁸) */
    seq_t f_sqrt5 = (seq_t)isqrt_uint64(sq5);

    /* Rational approximation of tan(pi/5) = 0.726542528005360... */
    static const unsigned __int128 TAN_NUM = (unsigned __int128)726542528005;
    static const unsigned __int128 TAN_DEN = (unsigned __int128)1000000000000ULL;

    unsigned __int128 prod = (unsigned __int128)nn * TAN_NUM;
    seq_t f_tan = (seq_t)(prod / TAN_DEN);

    return f_sqrt5 + f_tan;
}
