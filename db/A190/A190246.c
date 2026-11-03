/*
 * A190246  Positions of 0 in A190245; complement of A190247.
 * https://oeis.org/A190246
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 15, u32 n <= 3354, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 94 terms through n = 9864 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the nth integer (each ≥ 1) for which
   floor(3*sqrt(2)*n) = floor(sqrt(2)*n) + floor(2*sqrt(2)*n).
   The computation uses only integer arithmetic and the helper
   floor_sqrt(seq_t) to evaluate floor(sqrt(k*n^2)). */

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
    /* Verify that n is positive; we assume caller passes n ≥ 1. */
    if (n == 0) {
        return (seq_t)0;
    }

    idx_t i = 1;              /* Candidate integer n (index in OEIS) */
    idx_t cnt = 0;            /* Number of suitable integers found so far */

    for (;;) {
        /* Compute floor(sqrt(2)*i), floor(sqrt(8)*i), floor(sqrt(18)*i)
           using floor_sqrt on 2*i^2, 8*i^2, and 18*i^2. */
        seq_t sq = (seq_t)i * (seq_t)i;          /* i^2 */
        seq_t a = floor_sqrt( (seq_t)2 * sq );   /* floor(√(2) * i) */
        seq_t b = floor_sqrt( (seq_t)8 * sq );   /* floor(√(8) * i) */
        seq_t c = floor_sqrt( (seq_t)18 * sq );  /* floor(√(18) * i) */

        if (c == a + b) {
            cnt++;
            if (cnt == n) {
                return (seq_t)i;
            }
        }
        i++;
    }
}
