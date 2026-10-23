/*
 * A190491  a(n) = [(bn+c)r]-b[nr]-[cr], where (r,b,c)=(sqrt(2),3,1) and
 *          []=floor.
 * https://oeis.org/A190491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((3*n+1)*sqrt(2)) - 3*floor(n*sqrt(2)) - 1
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses only integer arithmetic and the provided integer types */

#include <stdint.h>

/* Integer square root for unsigned __int128 numbers */
/* Returns the greatest uint64_t r such that r*r <= x */
static uint64_t u128_sqrt(unsigned __int128 x)
{
    uint64_t low = 0;
    uint64_t high = (uint64_t)-1;          /* 2^64-1, maximum possible result */
    while (low <= high) {
        uint64_t mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == x)
            return mid;
        if (sq < x) {
            low = mid + 1;
        } else {
            if (mid == 0)
                break;
            high = mid - 1;
        }
    }
    return high;  /* high is the floor of sqrt(x) */
}

seq_t a(idx_t n)
{
    /* Compute floor((3n+1)*sqrt(2)) */
    idx_t k = 3 * n + 1;
    unsigned __int128 v1 = (unsigned __int128)2 * (unsigned __int128)k * k;
    uint64_t m1 = u128_sqrt(v1);

    /* Compute floor(n*sqrt(2)) */
    unsigned __int128 v2 = (unsigned __int128)2 * (unsigned __int128)n * n;
    uint64_t m2 = u128_sqrt(v2);

    /* a(n) = m1 - 3*m2 - 1, safely in 128-bit intermediate */
    unsigned __int128 tmp = (unsigned __int128)m1 - (unsigned __int128)3 * m2 - 1;

    return (seq_t)tmp;
}
