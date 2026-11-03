/*
 * A201206  Number of successive decreasing values of round(n^(2/3))^3 - n^2.
 * https://oeis.org/A201206
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n+1/2)^(3/2)) - floor((n-1/2)^(3/2))
 * Range:     u8 n <= 2, u16 n <= 19, u32 n <= 2600, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 91 terms through n = 9193 (2026-11-01)
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

/* Helper: compute f(n) = floor((n+1/2)^(3/2)) */
static inline seq_t floor_f(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t two_nn_plus_one = nn * 2 + 1;          /* 2n + 1 */
    seq_t V = two_nn_plus_one * two_nn_plus_one; /* (2n+1)^2 */
    V = V * two_nn_plus_one;                    /* (2n+1)^3 */
    seq_t V_div8 = V / ((seq_t)8);              /* floor(V / 8) */
    return floor_sqrt(V_div8);
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* a(n) = f(n) - f(n-1) where f is as defined above */
    return floor_f(n) - floor_f(n - 1);
}
