/*
 * A187341  Floor((5-sqrt(5))n); complement of A187342.
 * https://oeis.org/A187341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((5-sqrt(5)) * n)
 * Range:     u8 n <= 3, u16 n <= 57, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 61 (2026-11-01)
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
    /* cast index to seq_t for multiplication */
    seq_t nn = (seq_t)n;

    /* compute floor_sqrt(5 * n * n) */
    seq_t t = 5 * nn * nn;
    seq_t sqrt_term = floor_sqrt(t);

    /* a(n) = 5n - floor(n * sqrt(5)) - 1, where
       floor(n * sqrt(5)) = floor_sqrt(5*n*n) */
    seq_t result = 5 * nn - sqrt_term - 1;
    return result;
}
