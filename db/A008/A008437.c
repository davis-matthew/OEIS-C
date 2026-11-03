/*
 * A008437  Expansion of Jacobi theta constant theta_2^3 /8.
 * https://oeis.org/A008437
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{(x,y,z) ∈ ℕ⁺³ | x,y,z odd, x² + y² + z² = n}
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 9645 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of ordered triples of odd positive integers whose squares sum to n */

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
    /* Convert n to seq_t for arithmetic */
    seq_t n_seq = (seq_t)n;
    /* Max odd integer whose square ≤ n */
    idx_t limit = (idx_t)floor_sqrt(n_seq);

    seq_t count = 0;

    for (idx_t x = 1; x <= limit; x += 2) {
        seq_t x_sq = (seq_t)x * (seq_t)x;
        /* If x_sq alone already exceeds n, break */
        if (x_sq > n_seq) break;

        for (idx_t y = 1; y <= limit; y += 2) {
            seq_t y_sq = (seq_t)y * (seq_t)y;
            seq_t sum_xy = x_sq + y_sq;
            if (sum_xy > n_seq) continue;           /* y too large */

            seq_t rem = n_seq - sum_xy;             /* remainder for z² */
            if (rem == 0) continue;                 /* z must be positive */

            seq_t z = floor_sqrt(rem);
            if (z * z == rem && (z & 1))            /* z must be odd */
                count++;
        }
    }

    return count;
}
