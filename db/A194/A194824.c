/*
 * A194824  a(n) = 2+floor(Sum_{k=1..n} <((-1)^k)*k*sqrt(3)>), where < > =
 *          fractional part.
 * https://oeis.org/A194824
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 2 + floor( Σ_{k=1..n} (-1)^k * frac(k·√3) )
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where frac(x) = x - floor(x) is the fractional part of x. */

#include <stdint.h>
#include <stdbool.h>

/* Integer square root: floor( √x )  for x < 2⁶⁴ */
static inline uint64_t isqrt_uint64(uint64_t x)
{
    uint64_t lo = 0, hi = (x < ((uint64_t)1 << 32)) ? x : ((uint64_t)1 << 32) - 1;
    uint64_t ans = 0;
    while (lo <= hi)
    {
        uint64_t mid = (lo + hi) >> 1;
        uint64_t sq = mid * mid;
        if (sq <= x)
        {
            ans = mid;
            lo = mid + 1;
        }
        else
            hi = mid - 1;
    }
    return ans;
}

/* Compute a(n) for the OEIS sequence A194824 */
seq_t a(idx_t n)
{
    /* Compute B = Σ_{k=1..n} (-1)^k k */
    int64_t B;
    if ((n & 1ULL) == 0)          /* n even */
        B = (int64_t)(n / 2ULL);
    else                           /* n odd */
        B = - (int64_t)((n + 1ULL) / 2ULL);

    /* floor(B * √3) */
    seq_t floorB;
    if (B >= 0)
    {
        uint64_t B64 = (uint64_t)B;
        uint64_t val = isqrt_uint64(3ULL * B64 * B64);
        floorB = (seq_t)val;
    }
    else
    {
        uint64_t m = (uint64_t)(-B);
        uint64_t val = isqrt_uint64(3ULL * m * m);
        floorB = -( (seq_t)val + 1 );
    }

    /* Compute T2 = Σ_{k=1..n} (-1)^k floor(k·√3) */
    seq_t T2 = 0;
    for (idx_t k = 1; k <= n; ++k)
    {
        uint64_t valk = isqrt_uint64(3ULL * k * k);
        if (k & 1ULL)           /* k odd: sign = -1 */
            T2 -= (seq_t)valk;
        else                    /* k even: sign = +1 */
            T2 += (seq_t)valk;
    }

    /* Final result: a(n) = 2 + floor(B·√3) - T2 */
    return (seq_t)2 + floorB - T2;
}
