/*
 * A053479  Circle numbers (version 6): a(n) = number of points
 *          (i+j/2,j*sqrt(3)/2), i,j integers (triangular grid) contained in a
 *          circle of diameter n, centered at (1/2, 1/(2*sqrt(3))).
 * https://oeis.org/A053479
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of integer lattice points (s,t) with 12*(s^2 + t^2
 *            + s*t - s - t) <= 3*n^2 - 4. The inequality follows from the
 *            circle condition in the OEIS definition
 * Range:     u8 n <= 16, u16 n <= 268, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ceil_div128, floor_div128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/ceil_div128.h"
#include "../utils/floor_div128.h"

/* Helper functions for 128‑bit integer arithmetic */

static unsigned long long isqrt_u64(unsigned long long x)
/* floor(sqrt(x)) – binary search */
{
    unsigned long long r = 0, bit = 1ULL << 62;
    while (bit) {
        if (r + bit <= x) {
            x -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

/* ---------- sequence function ---------- */
seq_t a(idx_t n)
{
    /* 12 * left <= 3*n^2 - 4 */
    unsigned __int128 mfrac = (unsigned __int128)3 * n * n - 4;  /* RHS multiplier */
    seq_t result = 0;

    /* s ranges from -n to n; tighter bounds are possible but not needed */
    for (int64_t s = -(int64_t)n; s <= (int64_t)n; ++s) {
        /* compute coefficients of inequality in t:
           a = 12, b = 12*(s-1), c = 12*(s^2 - s) - mfrac <= 0
        */
        __int128 b = (__int128)12 * (s - 1);
        __int128 c = (__int128)12 * ((int64_t)s * (int64_t)s - s) - (__int128)mfrac;

        /* discriminant Δ = b^2 - 4*a*c = b^2 - 48*c */
        __int128 delta = b * b - (__int128)48 * c;
        if (delta < 0) {
            continue;               /* no real roots → no solutions */
        }

        /* floor sqrt of Δ */
        unsigned long long d_sqrt = isqrt_u64((unsigned long long)delta);

        /* roots numerator: -b ± sqrtΔ */
        __int128 t_min_num = -b - d_sqrt;
        __int128 t_max_num = -b + d_sqrt;

        /* divide by 2*a = 24, taking integer ceil/floor */
        __int128 t_min = ceil_div128(t_min_num, (__int128)24);
        __int128 t_max = floor_div128(t_max_num, (__int128)24);

        if (t_max >= t_min) {
            result += (seq_t)(t_max - t_min + 1);
        }
    }

    return result;
}
