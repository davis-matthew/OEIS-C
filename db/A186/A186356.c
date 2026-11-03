/*
 * A186356  Adjusted joint rank sequence of (f(i)) and (g(j)) with f(i) after
 *          g(j) when f(i)=g(j), where f(i)=3i and g(j)=j(j+1)/2 (triangular
 *          number). Complement of A186357.
 * https://oeis.org/A186356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor((sqrt(1 + 24*n) - 1)/2)
 * Range:     u8 n <= 2, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Validated: partial, pass, 120 terms through n = 120 (2026-11-01)
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
    /* Convert n to seq_t for arithmetic with the same type as seq_t. */
    seq_t N = (seq_t)n;

    /* Compute 1 + 24*n (must be done in seq_t to avoid overflow). */
    seq_t val = (seq_t)1 + (seq_t)24 * N;

    /* Floor square root of val. */
    seq_t s = floor_sqrt(val);

    /* j = floor((sqrt(1+24*n)-1)/2). */
    seq_t j = (s - (seq_t)1) / (seq_t)2;

    /* a(n) = n + j. */
    return N + j;
}
