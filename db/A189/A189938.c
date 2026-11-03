/*
 * A189938  a(n) = n + [n*r/s] + [n*t/s]; r=1, s=sin(Pi/8), t=cos(Pi/8).
 * https://oeis.org/A189938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(n*csc(pi/8)) + floor(n*cot(pi/8))
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 309, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 86 terms through n = 9520 (2026-11-01)
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

/* floor(n*csc(pi/8))  = floor(n*sqrt(4+2*sqrt(2)))  */
static inline seq_t floor_nalpha(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* Bounds for the integer part:
         4*n^2 < n^2*(4+2*sqrt(2)) < 7*n^2
         -> sqrt < sqrt(7)*n < 3*n  (for all n>=1) */
    idx_t low  = 0;
    idx_t high = 3 * n;
    idx_t best = 0;

    seq_t a = (seq_t)4 * n * n;   /* 4*n^2 */
    seq_t b = (seq_t)2 * n * n;   /* 2*n^2 */

    while (low <= high) {
        idx_t mid = low + (high - low) / 2;      /* avoid overflow */
        seq_t mid_sq = (seq_t)mid * mid;          /* mid^2        */

        /* Determine if mid <= sqrt(a + b*sqrt(2)) */
        int ok;                                 /* 1 if holds, -1 otherwise */
        if (mid_sq <= a) {
            ok = 1;
        } else {
            seq_t diff = mid_sq - a;                 /* diff > 0 */
            seq_t lhs  = diff * diff;                /* (mid^2 - a)^2   */
            seq_t rhs  = (seq_t)2 * b * b;           /* 2*b^2           */
            ok = (lhs <= rhs) ? 1 : -1;
        }

        if (ok > 0) {
            best = mid;
            low  = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return (seq_t)best;
}

/* floor(n*sqrt(2))  using integer arithmetic */
static inline seq_t floor_nsqrt2(idx_t n)
{
    seq_t nsq = (seq_t)n * n;   /* n^2   */
    seq_t val = nsq * (seq_t)2; /* 2*n^2 */
    return floor_sqrt(val);
}

/* Main sequence function for A189938 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n<1, guard against underflow */

    seq_t n_seq = (seq_t)n;
    seq_t alpha_floor = floor_nalpha(n);
    seq_t sqrt2_floor = floor_nsqrt2(n);

    /* a(n) = n + floor_nalpha + floor_nbeta
              = n + floor_nalpha + (n + floor_nsqrt2) */
    seq_t result = (seq_t)2 * n_seq + alpha_floor + sqrt2_floor;
    return result;
}
