/*
 * A190006  a(n) = n + [n*s/r] + [n*t/r]; r=1, s=sin(Pi/3), t=csc(Pi/3).
 * https://oeis.org/A190006
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(n*sin(Pi/3)) + floor(n*csc(Pi/3))
 * Range:     u8 n <= 84, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square root for unsigned __int128: floor(sqrt(x)) */
static unsigned __int128 isqrt_u128(unsigned __int128 x)
{
    unsigned __int128 low = 0;
    unsigned __int128 high = ((unsigned __int128)1 << 64) - 1; /* sqrt of max 128-bit */
    while (low < high)
    {
        unsigned __int128 mid = low + (high - low + 1) / 2;
        if (mid * mid <= x)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* compute floor(n*sqrt(3)) as floor(sqrt(3*n*n)) */
    unsigned __int128 prod1 = (unsigned __int128)n * n * 3;
    unsigned __int128 sqrt1 = isqrt_u128(prod1);          /* floor(n*sqrt(3))   */
    seq_t f1 = (seq_t)(sqrt1 / 2);                       /* floor(n*sqrt(3)/2) */

    /* compute floor(n*2/sqrt(3)) as floor(sqrt((4*n*n)/3)) */
    unsigned __int128 prod2 = (unsigned __int128)n * n * 4;
    unsigned __int128 div   = prod2 / 3;                  /* integer part of (4*n*n)/3 */
    unsigned __int128 sqrt2 = isqrt_u128(div);            /* floor(2*n/sqrt(3))  */
    seq_t f2 = (seq_t)sqrt2;

    return (seq_t)n + f1 + f2;
}
