/*
 * A188374  [nr+kr]-[nr]-[kr], where r=1/sqrt(2), k=2, [ ]=floor.
 * https://oeis.org/A188374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n+2)/sqrt(2)) - floor(n/sqrt(2)) - 1
 * Range:     u8 n <= 9, u16 n <= 728, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 171 terms through n = 9633 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Derived from a(n) = ⌊n r + 2 r⌋ − ⌊n r⌋ − ⌊2 r⌋ with r = 1/√2.
   Using the identity ⌊x/√2⌋ = ⌊√(x²/2)⌋. */
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
    /* floor(n/√2) = floor(√(n²/2)) */
    seq_t n_sq   = (seq_t)n * (seq_t)n;
    seq_t f1_sq  = n_sq / 2;            /* n² / 2 (integer division) */
    seq_t f1     = floor_sqrt(f1_sq);   /* floor(n/√2) */

    /* floor((n+2)/√2) = floor(√((n+2)²/2)) */
    seq_t np2    = n + (idx_t)2;
    seq_t np2_sq = (seq_t)np2 * (seq_t)np2;
    seq_t f2_sq  = np2_sq / 2;           /* (n+2)² / 2 (integer division) */
    seq_t f2     = floor_sqrt(f2_sq);    /* floor((n+2)/√2) */

    return f2 - f1 - 1;                  /* a(n) = f2 - f1 - 1 */
}
