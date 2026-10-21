/*
 * A089806  Expansion of Jacobi theta function
 *          (theta_3(q^(1/3))-theta_2(q^3))/2/q^(1/12).
 * https://oeis.org/A089806
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if 12*n+1 is a perfect square and not divisible by 3,
 *            otherwise 0
 * Range:     u8 n <= 9643, u16 n <= 9643, u32 n <= 9643, u64 n <= 9643,
 *            u128 n <= 9643, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static unsigned long long isqrt_uint128(unsigned __int128 x)
{
    unsigned __int128 lo = 0, hi = x;
    while (lo <= hi) {
        unsigned __int128 mid = (lo + hi) >> 1;
        unsigned __int128 sq  = mid * mid;
        if (sq == x)          return (unsigned long long)mid;
        if (sq < x)           lo = mid + 1;
        else                  hi = mid - 1;
    }
    return (unsigned long long)hi;           /* floor sqrt */
}

seq_t a(idx_t n)
{
    /* compute m = 12*n + 1 as 128-bit integer to avoid overflow */
    unsigned __int128 m = (unsigned __int128)n * 12 + 1;

    /* check perfect square */
    unsigned long long r = isqrt_uint128(m);
    if ((unsigned __int128)r * r != m)
        return (seq_t)0;

    /* check divisibility by 3 */
    if ((m % 3) == 0)
        return (seq_t)0;

    return (seq_t)1;
}
