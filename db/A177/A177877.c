/*
 * A177877  Triangle in which row n is derived from (1,2,3,...,n) dot
 *          (n,n-1,...,1) with additive carryovers.
 * https://oeis.org/A177877
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = ((k+1)(k+2)(3(r+1)-2k))/6 where r =
 *            floor((sqrt(8n+1)-1)/2), k = n - r(r+1)/2 This computes the
 *            row/column decomposition of the triangle defined by (1,2,…,r+1)
 *            dot (r+1,…,1) with additive carryovers
 * Range:     u8 n <= 54, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

seq_t a(idx_t n)
{
    /* 1. Find the row r such that r(r+1)/2 <= n < (r+1)(r+2)/2. */
    idx_t r = (idx_t)((sqrt((long double)(8ULL*n+1))-1)/2);
    /* Correct possible rounding errors that can happen with floating point. */
    while (( (__uint128_t)(r+1)*(r+2) ) / 2 <= n) r++;
    while (( (__uint128_t)r*(r+1) ) / 2 > n) r--;

    /* 2. Position of the term within its row. */
    idx_t offset = (idx_t)(( (__uint128_t)r*(r+1) ) / 2);
    idx_t k = n - offset;

    /* 3. Compute the value using the derived integer formula. */
    seq_t a1 = (seq_t)(k + 1);
    seq_t a2 = (seq_t)(k + 2);
    seq_t c  = (seq_t)(3*((seq_t)r + 1) - 2*(seq_t)k);
    return a1 * a2 * c / 6;
}
