/*
 * A184930  a(n) = n + [rn/t] + [sn/t] + [un/t], where []=floor and
 *          r=sin(Pi/2), s=sin(Pi/3), t=sin(Pi/4), u=sin(Pi/5).
 * https://oeis.org/A184930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊n·√2⌋ + ⌊n·√(3/2)⌋ + ⌊n·√(5–√5)/2⌋
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 120, u64 n <= 120, u128 n <= 120,
 *            big any n
 * Validated: partial, pass, 120 terms through n = 120 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The third term uses integer arithmetic; the fourth term is computed
   by binary search with an inequality that avoids negative
   intermediate values. */

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

/* Helper: test 5 n^4 + 4 x^4 >= 10 n^2 x^2
   which is equivalent to x <= n * √(5–√5)/2. */
static int ratio_leq(seq_t x, idx_t n)
{
    seq_t n2 = (seq_t)n * (seq_t)n;
    seq_t n4 = n2 * n2;
    seq_t x2 = x * x;
    seq_t x4 = x2 * x2;
    seq_t left  = (seq_t)5 * n4 + (seq_t)4 * x4;
    seq_t right = (seq_t)10 * n2 * x2;
    return left >= right;
}

/* Floor of n * √(5–√5)/2  using the inequality in ratio_leq. */
static seq_t floor_ratio(idx_t n)
{
    if (n == 0) return 0;
    seq_t lo = 0, hi = n;                /* 0 ≤ floor(…) ≤ n */
    while (lo < hi) {
        seq_t mid = (lo + hi + 1) >> 1;   /* upper mid */
        if (ratio_leq(mid, n))
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* First term: n itself */
    seq_t term1 = (seq_t)n;

    /* Second term: ⌊n·√2⌋ = ⌊√(2·n²)⌋ */
    seq_t n_sq = (seq_t)n * (seq_t)n;
    seq_t term2 = floor_sqrt((seq_t)2 * n_sq);

    /* Third term: ⌊n·√(3/2)⌋ = ⌊√(6·n²)⌋ / 2 */
    seq_t term3 = floor_sqrt((seq_t)6 * n_sq) >> 1;

    /* Fourth term: ⌊n·√(5–√5)/2⌋ */
    seq_t term4 = floor_ratio(n);

    return term1 + term2 + term3 + term4;
}
