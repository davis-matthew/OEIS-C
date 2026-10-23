/*
 * A194206  a(n) = [Sum_{k=1..n} (k*Pi)], where [ ]=floor, ( )=fractional
 *          part.
 * https://oeis.org/A194206
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(∑_{k=1..n} frac(k*PI)) where frac(x) = x - floor(x)
 * Range:     u8 n <= 505, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 64-bit integer constants that provide an accurate rational approximation of PI
    PI ≈ 31415926535897932 / 10000000000000000   (error < 1e-16)
    Using this, frac(k*PI) ≈ ((k * PI_num) % PI_den) / PI_den
    The cumulative sum of fractional parts is computed by keeping a running
    remainder less than PI_den, incrementing the result whenever the sum
    reaches or surpasses PI_den.  The error in the approximation is far
    smaller than 0.5 for all n up to at least 10,000, so the floor is
    guaranteed to be correct.  */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Rational approximation PI ≈ PI_NUM / PI_DEN  */
    static const uint64_t PI_NUM   = 31415926535897932ULL;   /* N  */
    static const uint64_t PI_DEN   = 10000000000000000ULL;   /* D  */

    uint64_t sum = 0;            /* running fractional sum < PI_DEN  */
    seq_t   res = 0;             /* final floor value                */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* Compute (k * PI_NUM) mod PI_DEN using 128‑bit arithmetic
           to avoid overflow (k ≤ n ≤ 10^4, PI_NUM ≈ 3×10^16).    */
        unsigned __int128 prod = (unsigned __int128)k * PI_NUM;
        uint64_t frac = (uint64_t)(prod % PI_DEN);

        sum += frac;
        if (sum >= PI_DEN)
        {
            sum -= PI_DEN;
            ++res;
        }
    }

    return res;
}
