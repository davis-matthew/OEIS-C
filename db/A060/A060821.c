/*
 * A060821  Triangle read by rows. T(n, k) are the coefficients of the Hermite
 *          polynomial of order n, for 0 <= k <= n.
 * https://oeis.org/A060821
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, nice
 * Formula:   not stated
 * Range:     i8 n <= 17, i16 n <= 47, i32 n <= 122, i64 n <= 356,
 *            i128 n <= 1086, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A060821 – triangle T(n,k) of Hermite polynomial coefficients,
    flattened by rows:  a(0)=1, a(1)=0, a(2)=2, a(3)=-2, …
    a(n) = T(r,k) where      r = floor((sqrt(8*n+1)-1)/2)
                                k = n - r*(r+1)/2
    T(r,k) = 0                               if r-k is odd
         = (-1)^((r-k)/2) * 2^k * r! /
           (k! * ((r-k)/2)! )                otherwise   */
#include <stdint.h>

static inline uint64_t triangular_root(uint64_t n)
{
    /* return greatest r with r*(r+1)/2 <= n  */
    uint64_t low = 0;
    uint64_t high = 1;
    while ( ((unsigned __int128)high * (high + 1) / 2) <= n )
        high <<= 1;                       /* exponential growth */
    while (low < high) {
        uint64_t mid = low + (high - low + 1) / 2;
        if ( ((unsigned __int128)mid * (mid + 1) / 2) <= n )
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* find row r and column k in the flattened triangle   */
    uint64_t r = triangular_root((uint64_t)n);
    uint64_t k = (uint64_t)n - (uint64_t)((unsigned __int128)r * (r + 1) / 2);

    /* if r-k is odd the term is zero */
    if ( ((r - k) & 1ULL) != 0 )
        return (seq_t)0;

    uint64_t m = (r - k) >> 1;          /* (r-k)/2 */

    /* compute the coefficient T(r,k)  */
    seq_t res = (seq_t)1;

    /* binomial part:  r! / (k! * (r-k)! )  */
    for (uint64_t i = 1; i <= k; ++i)
        res = res * (seq_t)(r - k + i) / (seq_t)i;   /* exact division */

    /* extra multiplicative factor (r-k)! / m! = (2m)! / m!  */
    for (uint64_t i = m + 1; i <= 2 * m; ++i)
        res = res * (seq_t)i;

    /* multiply by 2^k */
    for (uint64_t i = 0; i < k; ++i)
        res = res * (seq_t)2;

    /* apply the sign (-1)^m  */
    if ( (m & 1ULL) != 0 )
        res = -res;

    return res;
}
