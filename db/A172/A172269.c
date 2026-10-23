/*
 * A172269  a(n) = floor(n*(sqrt(7)-sqrt(2))).
 * https://oeis.org/A172269
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( n * ( sqrt(7) - sqrt(2) ) )
 * Range:     u8 n <= 207, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute floor(n*sqrt(k)) using integer arithmetic only. */
#include <stdint.h>
#include <limits.h>

/* Integer square root of an unsigned __int128 using binary search.
   Returns the largest x such that x*x <= input. */
static inline unsigned __int128 isqrt_uint128(unsigned __int128 v)
{
    unsigned __int128 low  = 0;
    unsigned __int128 high = (unsigned __int128)1 << 64;   /* 2^64 */
    while (low <= high) {
        unsigned __int128 mid = (low + high) >> 1;
        unsigned __int128 sq  = mid * mid;
        if (sq == v)
            return mid;
        if (sq < v) {
            low = mid + 1;
        } else {
            if (mid == 0)
                break;
            high = mid - 1;
        }
    }
    return high;
}

/* Main sequence function.  Works for any seq_t size that can hold the
   result or will truncate the value (tests only check values that fit
   in seq_t). */
seq_t a(idx_t n)
{
    unsigned __int128 n128 = n;
    unsigned __int128 n2   = n128 * n128;   /* n^2 */
    unsigned __int128 n4   = n2 * n2;        /* n^4 */

    /* floor(n*sqrt(7)) and floor(n*sqrt(2)) */
    unsigned __int128 s7 = isqrt_uint128((unsigned __int128)7 * n2);
    unsigned __int128 s2 = isqrt_uint128((unsigned __int128)2 * n2);
    unsigned __int128 D  = s7 - s2;          /* D = floor(n*sqrt7) - floor(n*sqrt2) */

    /* D^2 and D^4 needed for the sign test of the fractional part */
    unsigned __int128 D2 = D * D;
    unsigned __int128 D4 = D2 * D2;

    /* expr = 25*n^4 - 18*D^2*n^2 + D^4 */
    signed __int128 expr = (signed __int128)25 * n4
                         - (signed __int128)18 * (signed __int128)D2 * (signed __int128)n2
                         + (signed __int128)D4;

    /* If the fractional part of n*alpha is negative, subtract 1 from D. */
    if (expr < 0)
        D -= 1;

    return (seq_t)D;
}
