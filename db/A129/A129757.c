/*
 * A129757  a(n) = ceiling((2^n + 1 - 2*floor(2^(n/2)))/2).
 * https://oeis.org/A129757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = ceil((2^n + 1 - 2*floor(2^(n/2)))/2)
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Validated: partial, pass, 32 terms through n = 32 (2026-11-01)
 * Helpers:   ipow_i32, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow_i32.h"
#include "../utils/pow2.h"

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
    /* 2^n */
    seq_t pow2_n = pow2(n);

    /* floor(2^(n/2)) = floor(sqrt(2^n)) */
    seq_t fsqrt = floor_sqrt(pow2_n);

    /* numerator = 2^n + 1 - 2*floor_sqrt */
    seq_t numerator = pow2_n + 1;
    numerator -= (fsqrt << 1);

    /* ceil division by 2: (numerator + 1) >> 1 */
    return (numerator + 1) >> 1;
}
