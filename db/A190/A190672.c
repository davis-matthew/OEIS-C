/*
 * A190672  [(bn+c)r]-b[nr]-[cr], where (r,b,c)=(sqrt(3),2,1) and [ ]=floor.
 * https://oeis.org/A190672
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((2n+1)*sqrt(3)) - 2*floor(n*sqrt(3)) - 1
 * Range:     u8 n <= 132, u16 n <= 132, u32 n <= 132, u64 n <= 132,
 *            u128 n <= 132, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for unsigned __int128.
   Returns ⌊√x⌋. */
static inline unsigned __int128 floor_sqrt_uint128(unsigned __int128 x)
{
    unsigned __int128 lo = 0, hi = ((unsigned __int128)1 << 64) - 1;
    while (lo <= hi)
    {
        unsigned __int128 mid = (lo + hi) >> 1;
        unsigned __int128 sq  = mid * mid;
        if (sq <= x)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

seq_t a(idx_t n)
{
    unsigned __int128 nn = n;
    /* floor(n*sqrt(3)) = floor(sqrt(3*n*n)) */
    unsigned __int128 val1 = (unsigned __int128)3 * nn * nn;
    unsigned __int128 s1   = floor_sqrt_uint128(val1);

    unsigned __int128 m   = (unsigned __int128)2 * nn + 1;
    unsigned __int128 val2 = (unsigned __int128)3 * m * m;
    unsigned __int128 s2   = floor_sqrt_uint128(val2);

    unsigned __int128 diff = s2 - (unsigned __int128)2 * s1 - 1;
    return (seq_t)diff;
}
