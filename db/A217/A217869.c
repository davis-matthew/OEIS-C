/*
 * A217869  a(n) is the sum of total number of positive integer solutions to
 *          each of a^2 + b^2 = n, a^2 + 2*b^2 = n, a^2 + 3*b^2 = n and a^2 +
 *          7*b^2 = n. (Order matters for the equation a^2+b^2 = n).
 * https://oeis.org/A217869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k ∈ {1,2,3,7}} #{(a,b) ∈ ℕ² | a>0, b>0, a² + k·b² =
 *            n}
 * Range:     u8 n <= 63, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Validated: partial, pass, 100 terms through n = 100 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Ordered pairs are counted for k = 1) */

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

seq_t a(idx_t n) {
    seq_t total = 0;
    static const idx_t ks[4] = {1, 2, 3, 7};

    for (idx_t i = 0; i < 4; ++i) {
        idx_t k = ks[i];
        seq_t t = ((seq_t)n) / ((seq_t)k);
        idx_t bmax = (idx_t)floor_sqrt(t);

        for (idx_t b = 1; b <= bmax; ++b) {
            seq_t b2 = (seq_t)b * (seq_t)b;
            seq_t r = ((seq_t)n) - ((seq_t)k) * b2;
            if (r == 0) continue;                 // requires a > 0

            seq_t a_val = floor_sqrt(r);
            if (a_val * a_val == r && a_val > 0) {
                total += 1;
            }
        }
    }
    return total;
}
