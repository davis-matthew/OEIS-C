/*
 * A155689  a(n) = Sum_{k >= 1} floor(n * sqrt(2) / 2^k).
 * https://oeis.org/A155689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k >= 1} floor(n * sqrt(2) / 2^k)
 * Range:     u8 n <= 5, u16 n <= 72, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 88 terms through n = 937 (2026-11-01)
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

/* Computes the nth term of sequence A155689.
   Uses the identity a(n) = floor_sqrt(2*n*n) - popcount(floor_sqrt(2*n*n)).
   popcount is computed by repeatedly clearing the lowest set bit. */
seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1.  Return 0 for safety. */
    if (n == 0) return 0;

    seq_t nn = (seq_t)n;
    seq_t M  = (seq_t)2 * nn * nn;          /* M = 2 * n * n */
    seq_t t  = floor_sqrt(M);              /* t = floor(n * sqrt(2)) */
    seq_t pc = 0;                          /* popcount of t */
    seq_t v  = t;
    while (v) {
        pc++;
        v &= v - 1;                       /* clear lowest set bit */
    }
    return t - pc;                        /* a(n) */
}
