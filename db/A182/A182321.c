/*
 * A182321  Number of iterations of A025581(n) required to reach 0.
 * https://oeis.org/A182321
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 83 terms through n = 874 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = number of iterations of g(n) = round(sqrt(2n))*(round(sqrt(2n))+1)/2 - n
 * needed to reach 0, where round(sqrt(2n)) = floor_sqrt(2n) rounded to nearest integer.
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
    seq_t count = 0U;

    while (n > 0U) {
        /* compute f = round(sqrt(2*n)) */
        seq_t two_n     = (seq_t)n * 2U;
        seq_t f         = floor_sqrt(two_n);          /* floor sqrt(2n)       */
        seq_t diff      = two_n - f * f;              /* 2n – f²              */
        seq_t twice_f_p1 = f * 2U + 1U;                /* 2f + 1               */
        if (diff * 2U >= twice_f_p1)                  /* round up if needed   */
            f += 1U;

        /* g(n) = f*(f+1)/2 - n */
        seq_t g = f * (f + 1U) / 2U - (seq_t)n;

        n = (idx_t)g;   /* g ≤ f, so it fits in idx_t */
        count++;
    }

    return count;
}
