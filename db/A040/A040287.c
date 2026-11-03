/*
 * A040287  Continued fraction for sqrt(305).
 * https://oeis.org/A040287
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0)=floor(sqrt(305))=17; for n≥1, a(n) is given by the standard
 *            recurrence for the continued fraction of sqrt(305): let m₀=0,
 *            d₀=1 and a₀=floor(sqrt(305)). For k≥0: mₖ₊₁ = dₖ·aₖ − mₖ dₖ₊₁ =
 *            (305 − mₖ₊₁²) / dₖ aₖ₊₁ = ⌊(a₀ + mₖ₊₁) / dₖ₊₁⌋ The resulting
 *            sequence is 17, 2, 6, 2, 34, 2, 6, 2, 34, … (...
 * Range:     u8 none, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Validated: partial, pass, 84 terms through n = 83 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
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
    const seq_t D = 305;
    seq_t a0 = floor_sqrt(D);
    if (n == 0)
        return a0;

    seq_t m = 0;
    seq_t d = 1;
    seq_t a_cur = a0;
    idx_t i;

    for (i = 1; i <= n; ++i) {
        /* compute next term in the continued fraction */
        m = d * a_cur - m;
        d = (D - m * m) / d;
        a_cur = (a0 + m) / d;
        if (i == n)
            return a_cur;
    }
    /* unreachable but keep compiler happy */
    return a_cur;
}
