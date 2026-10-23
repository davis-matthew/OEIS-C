/*
 * A101360  a(n) = ceiling((3-sqrt(3))*4^(n-3)) + 1.
 * https://oeis.org/A101360
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = ceil((3-sqrt(3))*4^(n-3)) + 1
 */
#include <stdint.h>
#include "../utils/pow2.h"

/* Integer square root for unsigned __int128.
   Returns floor(sqrt(v)).  The result always fits in a uint64_t
   because v <= 3*2^(124) < 2^126. */
static seq_t isqrt128(unsigned __int128 v)
{
    unsigned __int128 low  = 0;
    unsigned __int128 high = ((unsigned __int128)1) << 64;   /* exclusive upper bound */
    seq_t res = 0;

    while (low < high)
    {
        unsigned __int128 mid = (low + high) >> 1;
        unsigned __int128 sq  = mid * mid;
        if (sq <= v)
        {
            res = (seq_t)mid;
            low = mid + 1;
        }
        else
        {
            high = mid;
        }
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n < 3)
        return (seq_t)2;

    /* t = 4^(n-3) = 2^(2n-6) */
    idx_t e = (n << 1) - 6;
    seq_t t = pow2(e);                          /* 4^(n-3) */

    /* floor(t * sqrt(3)) = floor(sqrt(3 * t^2)) */
    unsigned __int128 t128 = (unsigned __int128)t;
    unsigned __int128 v = t128 * t128 * 3;      /* t^2 * 3 */
    seq_t floor_sqrt3_t = isqrt128(v);

    /* a(n) = 3*t - floor(t*sqrt(3)) + 1 */
    seq_t result = t * 3 - floor_sqrt3_t + 1;
    return result;
}
