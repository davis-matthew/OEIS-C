/*
 * A164272  Expansion of phi(q) * phi(-q^3) in powers of q where phi() is a
 *          Ramanujan theta function.
 * https://oeis.org/A164272
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   * a(n) = sum_{b=0}^{⌊√(n/3)⌋} * w(b, n), where * w(b, n) = (b==0
 *            ? (-1)^b * (r==0?1:2) * : 2*(-1)^b * (r==0?1:2)) * with r =
 *            n-3*b^2. * The term is included only if r is a perfect square. *
 *            This follows from φ(q)·φ(−q^3)=∑_{a,b∈ℤ}(-1)^b q^{a^2+3b^2}
 * Range:     i8 n <= 63, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Validated: standard, pass, 121 terms through n = 969 (2026-11-01)
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
    seq_t total = 0;

    /* maximum b such that 3*b*b ≤ n */
    idx_t max_b = (idx_t)floor_sqrt((seq_t)(n / 3));

    for (idx_t b = 0; b <= max_b; ++b)
    {
        idx_t b_sq = b * b;
        idx_t rem  = n - 3 * b_sq;       /* n – 3b², always ≥ 0 */

        /* check if rem is a perfect square */
        idx_t k = (idx_t)floor_sqrt((seq_t)rem);
        if (k * k != rem)
            continue;                  /* not a perfect square → skip */

        seq_t sign = (b & 1) ? (seq_t)-1 : (seq_t)1;
        seq_t cnt  = (rem == 0) ? (seq_t)1 : (seq_t)2;

        seq_t contrib = (b == 0) ? sign * cnt : sign * cnt * 2;
        total += contrib;
    }

    return total;
}
