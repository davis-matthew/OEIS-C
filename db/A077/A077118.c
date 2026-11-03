/*
 * A077118  Nearest integer square to n^3.
 * https://oeis.org/A077118
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nearest square to n^3 = (round(n^(3/2)))^2
 * Range:     u8 n <= 3, u16 n <= 25, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 40 terms through n = 39 (2026-11-01)
 * Helpers:   ipow, ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"
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
    seq_t nn   = (seq_t)n;
    seq_t cube = ipow(nn, (seq_t)3);

    seq_t r     = floor_sqrt(cube);
    seq_t diff1 = cube - r * r;

    seq_t r1    = r + (seq_t)1;
    seq_t diff2 = r1 * r1 - cube;

    if (diff1 <= diff2)
        return r * r;
    else
        return r1 * r1;
}
