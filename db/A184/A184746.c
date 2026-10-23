/*
 * A184746  floor(nr+h), where r=1+1/sqrt(5), h=1/2; complement of A184747.
 * https://oeis.org/A184746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*(1 + 1/sqrt(5)) + 1/2)
 * Range:     u8 n <= 120, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>


/* Integer square root for 128‑bit unsigned values */
static unsigned long long isqrt128(unsigned __int128 x)
{
    unsigned long long lo = 0, hi = (unsigned long long)-1;
    while (lo <= hi) {
        unsigned long long mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= x)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;               /* floor(sqrt(x)) */
}

seq_t a(idx_t n)
{
    /* a(n) = n + floor((2n*sqrt(5) + 5) / 10) */
    unsigned __int128 nn = n;
    unsigned __int128 two_n = nn << 1;                     /* 2*n */
    unsigned __int128 prod = (unsigned __int128)5 * two_n * two_n; /* 5*(2n)^2 */

    unsigned long long sqrt_val = isqrt128(prod);          /* floor(2n*sqrt(5)) */
    unsigned long long m = (sqrt_val + 5) / 10;            /* floor((2n*sqrt(5)+5)/10) */

    return (seq_t)n + (seq_t)m;
}
