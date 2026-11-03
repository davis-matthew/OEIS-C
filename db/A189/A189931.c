/*
 * A189931  c(n) = n + [n*r/t] + [n*s/t]; r=1, s=sin(2pi/5), t=cos(2pi/5).
 * https://oeis.org/A189931
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   c(n) = n + ⌊n·sec(2π/5)⌋ + ⌊n·tan(2π/5)⌋ sec(2π/5) = 1 + √5 →
 *            ⌊n·sec(2π/5)⌋ = n + ⌊n√5⌋ tan(2π/5) = √(5 + 2√5) (irrational) We
 *            compute ⌊n√5⌋ exactly by integer square root and ⌊n·tan(2π/5)⌋
 *            by a sufficiently accurate rational approximation
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Validated: standard, pass, 93 terms through n = 8877 (2026-11-01)
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

/* Rational approximation of tan(2π/5) = 3.077683537... */
#define _TAN_DEN 1000000000ULL   /* denominator 10^9  */
#define _TAN_NUM 3077683537ULL   /* floor(tan * 10^9) */

/* Compute the nth term of A189931 */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined for n = 0 */

    seq_t N = (seq_t)n;

    /* floor(n*sqrt(5)) = floor_sqrt(5 * n^2) */
    seq_t sq = 5 * N * N;
    seq_t f_sqrt5 = floor_sqrt(sq);

    /* floor(n*tan(2π/5)) using the rational approximation */
    seq_t f_n_tan = (N * (seq_t)_TAN_NUM) / (seq_t)_TAN_DEN;

    /* c(n) = 2n + floor(n√5) + floor(n*tan(2π/5)) */
    return 2 * N + f_sqrt5 + f_n_tan;
}
