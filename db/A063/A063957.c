/*
 * A063957  Numbers not of the form round(m*sqrt(2)) for any integer m, i.e.,
 *          complement of A022846.
 * https://oeis.org/A063957
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round((n - 1/2)*(2 + sqrt(2))) Equivalent integer
 *            formula: a(n) = ( floor((2n-1)*sqrt(2)) + 4n - 1 ) / 2 This is
 *            computed using only integer arithmetic and integer sqrt
 * Range:     u8 n <= 3, u16 n <= 45, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 75 terms through n = 963 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    /* N = 2n - 1 (odd) */
    seq_t N = (seq_t)2 * (seq_t)n - 1;
    /* t = floor(N * sqrt(2)) computed as floor(sqrt(2*N^2)) */
    seq_t t = floor_sqrt((seq_t)2 * N * N);
    /* a(n) = (t + 4n - 1) / 2  (integer division gives floor) */
    seq_t result = (t + (seq_t)4 * (seq_t)n - 1) / 2;
    return result;
}
