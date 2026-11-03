/*
 * A030204  Expansion of q^(-1/8) * eta(q) * eta(q^2) in powers of q.
 * https://oeis.org/A030204
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 63, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 96 terms through n = 9801 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A030204
 *  Formula: a(n) is the coefficient of qⁿ in
 *  ∏_{k>0} (1 - qᵏ)(1 - q^{2k})  = (∑_{k≥1} q^{k(k‑1)/2}) ·
 *  (∑_{k∈ℤ} (-1)ᵏ q^{k²}).
 *  The first factor has coefficient 1 for triangular numbers and 0 otherwise.
 *  The second factor has coefficient 1 at 0, 2·(-1)^m at m² and 0 otherwise.
 *  Thus
 *      a(n) = d(n) + Σ_{k≥2, k(k‑1)/2 ≤ n} d(n – k(k‑1)/2)
 *  where   d(m) = 1                    if m = 0
 *           d(m) = 2·(-1)^{√m}      if m is a perfect square
 *           d(m) = 0                otherwise.
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

static seq_t div_square_root(seq_t m)
{
    seq_t s = floor_sqrt(m);
    if (s * s != m) return (seq_t)0;
    return ((s & 1) == 0) ? (seq_t)2 : (seq_t)(-2);
}

static seq_t d(seq_t m)
{
    if (m == 0) return (seq_t)1;
    return div_square_root(m);
}

seq_t a(idx_t n)
{
    seq_t res = d((seq_t)n);

    for (idx_t k = 2; ; ++k) {
        seq_t t = ((seq_t)k * (k - 1)) / 2;
        if (t > (seq_t)n) break;
        idx_t diff = n - (idx_t)t;
        res += d((seq_t)diff);
    }

    return res;
}
