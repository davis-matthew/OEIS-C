/*
 * A082389  a(n) = floor((n+2)*phi) - floor((n+1)*phi) where
 *          phi=(1+sqrt(5))/2.
 * https://oeis.org/A082389
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = floor((n+2)*phi) - floor((n+1)*phi) (phi = (1+sqrt(5))/2)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root of a 128‑bit unsigned integer:
   returns floor(sqrt(x)).  Implementation uses binary search. */
static inline unsigned __int128 isqrt128(unsigned __int128 x)
{
    unsigned __int128 low = 0, high = (unsigned __int128)1 << 64; /* sqrt(2^128-1) < 2^64 */
    while (low < high) {
        unsigned __int128 mid = (low + high + 1) >> 1;
        if (mid * mid <= x)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* floor(n*phi) for n >= 0, using integer arithmetic:
   floor(n*phi) = floor((n + floor(n*sqrt(5))) / 2)  */
static inline seq_t floor_phi(idx_t n)
{
    /* compute floor(n*sqrt(5)) = floor(sqrt(5*n*n)) */
    unsigned __int128 prod = (unsigned __int128)n * n * (unsigned __int128)5;
    unsigned __int128 sq = isqrt128(prod);
    /* (n + sq) is at most 2*floor(n*phi) + 1 < 2^64 for n within practical range,
       so casting to seq_t is safe for the relevant builds. */
    return ((seq_t)n + (seq_t)sq) / 2;
}

seq_t a(idx_t n)
{
    /* n is 1‑based. Compute floor((n+2)*phi) and floor((n+1)*phi). */
    seq_t val1 = floor_phi(n + 2);
    seq_t val2 = floor_phi(n + 1);
    return val1 - val2;                /* difference is 1 or 2 */
}
