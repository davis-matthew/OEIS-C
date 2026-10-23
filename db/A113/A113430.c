/*
 * A113430  Expansion of f(-x, -x^2) * f(-x^10, -x^20) / f(-x^2, -x^8) in
 *          powers of x where f(, ) is Ramanujan's general theta function.
 * https://oeis.org/A113430
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = #{k∈ℤ | n = (15k² + k)/2} – #{k∈ℤ | n = (15k² –11k +
 *            2)/2} (the sequence values are −1, 0 or 1)
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

seq_t a(idx_t n)
{
    seq_t result = 0;                     /* possible values are −1, 0 or 1 */
    /* Upper bound for |k| :  (15/2)k² ≈ n  ⇒  |k| ≈ sqrt(2n/15) */
    long double bound = sqrtl((long double)n * 2.0L / 15.0L);
    long long kmax = (long long)(bound) + 2;   /* safety margin */

    for (long long k = -kmax; k <= kmax; ++k)
    {
        __int128 dk   = k;                               /* signed 128‑bit for safety   */
        __int128 e1   = (15 * dk * dk + dk) / 2;          /* (15k² + k)/2                */
        if (e1 == (__int128)n) result += 1;

        __int128 e2   = (15 * dk * dk - 11 * dk + 2) / 2; /* (15k² –11k + 2)/2           */
        if (e2 == (__int128)n) result -= 1;
    }
    return result;
}
