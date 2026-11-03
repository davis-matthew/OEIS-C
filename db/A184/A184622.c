/*
 * A184622  a(n) = floor(n*r+h), where r=sqrt(2), h=-1/3; complement of
 *          A184623.
 * https://oeis.org/A184622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*sqrt(2) - 1/3)
 * Range:     u8 n <= 5, u16 n <= 90, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 136 terms through n = 9364 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes floor(sqrt(2*n^2)) and adjusts based on the fractional part. */
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
    /* Convert index to seq_t for arithmetic. */
    seq_t nn = (seq_t)n;

    /* M = 2 * n^2 */
    seq_t M = 2 * nn * nn;

    /* k = floor(sqrt(M)) = floor(n * sqrt(2)) */
    seq_t k = floor_sqrt(M);

    /* Check if fractional part of sqrt(M) (i.e., sqrt(2)*n) is >= 1/3. */
    /* 9*M >= 9*k*k + 6*k + 1  <=>  sqrt(M) >= k + 1/3  */
    seq_t left  = 9 * M;
    seq_t right = 9 * k * k + 6 * k + 1;

    if (left >= right)
        return k;
    else
        return k - 1;
}
