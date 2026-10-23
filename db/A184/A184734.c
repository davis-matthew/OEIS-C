/*
 * A184734  a(n)=floor(n*r+h), where r=(1+sqrt(5))/2, h=1/3; complement of
 *          A184735.
 * https://oeis.org/A184734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*phi + 1/3), where phi = (1+sqrt(5))/2, h = 1/3
 * Range:     u8 n <= 114, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes floor(n*phi + 1/3) using only integer arithmetic.  */

#include <stdint.h>
#include <stdbool.h>

static inline unsigned long long isqrt_u128(unsigned __int128 x)
/* Return floor( sqrt(x) ) for 0 <= x < 2^128.  */
{
    unsigned long long lo = 0, hi = ~0ULL, ans = 0;
    while (lo <= hi) {
        unsigned long long mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= x) {
            ans = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    /* 1. Compute b = floor(n*sqrt(5)) as floor( sqrt(5*n*n) ) */
    unsigned __int128 n128 = (unsigned __int128)n;
    unsigned __int128 five_nn = (unsigned __int128)5 * n128 * n128;
    unsigned long long b = isqrt_u128(five_nn);   /* floor(n*sqrt(5)) */

    /* 2. Determine if the fractional part of n*phi is >= 2/3.
       This happens iff (n + b) is odd AND 45*n^2 >= (3*b + 1)^2.   */
    bool odd = ((n & 1) ^ (b & 1));   /* (n + b) odd  */
    unsigned __int128 left  = (unsigned __int128)45 * n128 * n128;
    unsigned __int128 rnum  = (unsigned __int128)(3 * b + 1);
    unsigned __int128 right = rnum * rnum;
    bool inc = odd && (left >= right);

    /* 3. Floor(n*phi) = floor((n + b)/2)  */
    seq_t floor_phi = ((seq_t)n + (seq_t)b) / 2;

    /* 4. a(n) = floor(n*phi) + 1 if inc else floor(n*phi) */
    return floor_phi + (seq_t)inc;
}
