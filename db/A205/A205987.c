/*
 * A205987  G.f.: Sum_{n=-oo..oo} q^(9n^2+8n).
 * https://oeis.org/A205987
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if n can be written as 9k²+8k for some integer k,
 *            otherwise 0
 * Range:     u8 none, u16 n <= 386, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 137 terms through n = 954 (2026-11-01)
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

seq_t a(idx_t n) {
    /* 9k^2 + 8k = n  <=>  36k^2 + 32k - 4n = 0
       discriminant D = 64 + 36n must be a perfect square, say s^2
       Then k = (-8 ± s)/18  should be integral */
    seq_t D = (seq_t)36 * (seq_t)n + (seq_t)64;
    seq_t s = floor_sqrt(D);
    if (s * s != D) return (seq_t)0;

    /* Check the two possible solutions */
    if ( (s - (seq_t)8) % (seq_t)18 == 0 ) return (seq_t)1;
    if ( (s + (seq_t)8) % (seq_t)18 == 0 ) return (seq_t)1;
    return (seq_t)0;
}
