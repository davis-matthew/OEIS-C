/*
 * A194151  Beatty sequence for (1/2)*sqrt(5); complement of A194152.
 * https://oeis.org/A194151
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⌊n * √5 / 2⌋ (Beatty sequence for (1/2)·√5)
 * Range:     u8 n <= 3, u16 n <= 57, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Validated: partial, pass, 69 terms through n = 69 (2026-11-01)
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
    /* Compute floor(√(5*n*n)) and then take integer division by 2. */
    seq_t nn   = (seq_t)n * (seq_t)n;   /* n² */
    seq_t prod = (seq_t)5 * nn;         /* 5·n² */
    seq_t s    = floor_sqrt(prod);      /* ⌊√(5·n²)⌋ = ⌊n·√5⌋ */
    return s / 2;                       /* ⌊⌊n·√5⌋ / 2⌋ = desired value */
}
