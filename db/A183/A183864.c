/*
 * A183864  a(n) = n + floor(sqrt(5*n/3)); complement of A183865.
 * https://oeis.org/A183864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(sqrt(5*n/3))
 * Range:     u8 n <= 12, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Validated: partial, pass, 100 terms through n = 100 (2026-11-01)
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
    seq_t N      = (seq_t)n;   /* cast to seq_t for big‑int safety          */
    seq_t fiveN  = N * (seq_t)5;/* 5*n                                           */
    /* binary search for the largest x obeying 3*x*x <= 5*n */
    seq_t low  = 0;
    /* upper bound: sqrt(5*n) (since 3*x*x <= 5*n implies x <= sqrt(5*n)) */
    seq_t high = floor_sqrt(fiveN);
    seq_t mid, floor_sqrt_val;

    while (low <= high)
    {
        mid = (low + high) / 2;            /* mid = floor((low+high)/2)          */
        seq_t lhs = mid * mid;             /* mid*mid                               */
        lhs *= (seq_t)3;                   /* 3*mid*mid                            */
        if (lhs <= fiveN)
        {
            low = mid + 1;                  /* keep mid as candidate, search higher */
        }
        else
        {
            high = mid - 1;                 /* search lower                           */
        }
    }

    floor_sqrt_val = high;                /* last valid mid                        */
    return N + floor_sqrt_val;             /* n + floor(sqrt(5*n/3))                */
}
