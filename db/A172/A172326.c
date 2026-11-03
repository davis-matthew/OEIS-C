/*
 * A172326  a(n) = floor(n*(sqrt(7) + sqrt(2))).
 * https://oeis.org/A172326
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = floor( n*(sqrt(7) + sqrt(2)) )
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 300, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 67 terms through n = 969 (2026-11-01)
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
    // n^2
    seq_t nl = (seq_t)n * (seq_t)n;

    // floor(n*sqrt(7)) and floor(n*sqrt(2))
    seq_t sqrt7 = floor_sqrt((seq_t)7 * nl);
    seq_t sqrt2 = floor_sqrt((seq_t)2 * nl);

    seq_t S   = sqrt7 + sqrt2;          // floor(n*sqrt7) + floor(n*sqrt2)
    seq_t Sp1 = S + (seq_t)1;           // S + 1

    // D = (S + 1)^2 - 9*n^2  (always non‑negative)
    seq_t D = Sp1 * Sp1 - (seq_t)9 * nl;

    // Compare D^2 with 56 * n^4
    seq_t left  = D * D;                  // D^2
    seq_t right = (seq_t)56 * nl * nl;    // 56 * n^4

    if (left <= right)
        return Sp1;   // floor(n*sqrt(7)+sqrt(2)) = S + 1
    else
        return S;     // floor(n*sqrt(7)+sqrt(2)) = S
}
