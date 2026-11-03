/*
 * A185595  a(n) = floor(n^(3/2)) - floor(n^(1/2)); complement of A185596.
 * https://oeis.org/A185595
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n^(3/2)) - floor(n^(1/2))
 * Range:     u8 n <= 3, u16 n <= 25, u32 n <= 345, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 96 terms through n = 4968 (2026-11-01)
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
    if (n == 0) {                 // offset is 1, behavior for n=0 is irrelevant
        return (seq_t)0;
    }
    seq_t ns = (seq_t)n;          // n converted once to seq_t
    seq_t sqrt_n   = floor_sqrt(ns);   // floor(sqrt(n))

    seq_t n_sq = ns * ns;         // n^2
    seq_t n_cu = n_sq * ns;       // n^3

    seq_t sqrt_n3 = floor_sqrt(n_cu);   // floor(sqrt(n^3)) == floor(n^(3/2))

    return sqrt_n3 - sqrt_n;      // final result
}
