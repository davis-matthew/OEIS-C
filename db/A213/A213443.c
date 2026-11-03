/*
 * A213443  a(0)=5, thereafter a(n) = chromatic number (or Heawood number)
 *          Chi(n) of surface of genus n.
 * https://oeis.org/A213443
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n==0? 5 : ⌊(7 + √(1 + 48 n))/2⌋)
 * Range:     u8 n <= 1, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 69 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is the Heawood number for orientable surfaces of genus n,
   with a special case for n = 0 returning 5. */
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
    if (n == (idx_t)0) {
        return (seq_t)5;
    }
    seq_t m = (seq_t)n;
    seq_t val = m * (seq_t)48 + (seq_t)1;
    seq_t s   = floor_sqrt(val);
    seq_t num = (seq_t)7 + s;
    return num / (seq_t)2;
}
