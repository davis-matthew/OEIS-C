/*
 * A134917  a(n) = ceiling(n^(4/3)).
 * https://oeis.org/A134917
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(n^(4/3))
 * Range:     u8 n <= 63, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer cube root: floor(cuberoot(x)) for 0 <= x <= 2^64-1 */
static unsigned long long cube_root_floor(unsigned long long x)
{
    unsigned long long low = 0;
    unsigned long long high = 1;
    /* grow high until it cannot fit the cube of x */
    while (((unsigned __int128)high * high * high) < x)
        high <<= 1;

    while (low < high) {
        unsigned long long mid = (low + high + 1) >> 1;
        unsigned __int128 m3 = (unsigned __int128)mid * mid * mid;
        if (m3 <= x)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    unsigned long long n64 = (unsigned long long)n;
    unsigned long long n2  = n64 * n64;   /* n^2  */
    unsigned long long n4  = n2 * n2;     /* n^4  */

    unsigned long long cbrt = cube_root_floor(n4);        /* floor of cube root */
    unsigned __int128 cbrt3 = (unsigned __int128)cbrt * cbrt * cbrt;

    unsigned long long res = (cbrt3 == n4) ? cbrt : cbrt + 1; /* ceil of n^(4/3) */
    return (seq_t)res;
}
