/*
 * A054533  Triangular array giving Ramanujan sum T(n,k) = c_n(k) =
 *          Sum_{m=1..n, (m,n)=1} exp(2 Pi i m k / n) for n >= 1 and 1 <= k <=
 *          n.
 * https://oeis.org/A054533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, nice, tabl
 * Formula:   a(n) = T(r,k) where r = row index, k = column index: r is the
 *            unique integer with r(r-1)/2 < n ≤ r(r+1)/2, k = n – r(r-1)/2,
 *            T(r,k) = Σ_{d | gcd(r,k)} d * μ(r/d) (µ = Möbius function)
 * Range:     i8 n <= 8645, i16 n <= 9870, i32 n <= 9870, i64 n <= 9870,
 *            i128 n <= 9870, big any n
 * Helpers:   mobius, mygcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mobius.h"      /* provides seq_t mobius(idx_t) */
#include "../utils/mygcd.h"

/* Integer triangular number r*(r+1)/2, computed safely */
static inline idx_t triangular(idx_t r)
{
    return (idx_t)(((unsigned __int128)r * (r + 1u)) / 2u);
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* sequence is 1‑based */

    /* Find row r such that triangular(r-1) < n ≤ triangular(r) */
    idx_t lo = 1, hi = 1;
    while (triangular(hi) < n)
        hi <<= 1;                      /* exponential search for upper bound */

    while (lo < hi) {                  /* binary search on [lo, hi] */
        idx_t mid = lo + (hi - lo) / 2;
        if (triangular(mid) >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t r = lo;
    idx_t prev_tri = triangular(r - 1u);
    idx_t k = n - prev_tri;            /* column index (1‑based) */

    /* Compute T(r,k) = Σ_{d | gcd(r,k)} d * μ(r/d) */
    idx_t g = mygcd(r, k);
    seq_t sum = 0;

    for (idx_t d = 1; d * d <= g; ++d) {
        if (g % d == 0) {
            seq_t term = (seq_t)d * mobius(r / d);
            sum += term;
            if (d != g / d) {
                idx_t d2 = g / d;
                term = (seq_t)d2 * mobius(r / d2);
                sum += term;
            }
        }
    }
    return sum;
}
