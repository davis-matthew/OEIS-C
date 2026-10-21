/*
 * A230209  Trapezoid of dot products of row 6 (signs alternating) with
 *          sequential 7-tuples read by rows in Pascal's triangle A007318:
 *          T(n,k) is the linear combination of the 7-tuples (C(6,0), -C(6,1),
 *          ..., -C(6,5), C(6,6)) and (C(n-1,k-6), C(n-1,k-5), ..., C(n-1,k)),
 *          n >= 1, 0 <= k <= n+5.
 * https://oeis.org/A230209
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign, tabf
 * Formula:   a(n) enumerates the trapezoid T(r,c) where T(r,c) = Σ_{i=0}^6
 *            (-1)^i·C(6,i)·C(r-1,c-i), with 0 ≤ c ≤ r+5, r ≥ 1. a(n) is the
 *            row‑major traversal of T starting from r=1, c=0
 * Range:     i8 n <= 93, i16 n <= 221, i32 n <= 786, i64 n <= 1575,
 *            i128 n <= 1575, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include <stddef.h>

static const seq_t bin6[7] = { 1, -6, 15, -20, 15, -6, 1 };

seq_t a(idx_t n)
{
    /* Compute row r such that cumulative rows up to r contain index n */
    idx_t cumulative = 0;
    idx_t r = 0;
    for (r = 1; ; ++r) {
        cumulative += r + 6;          /* length of row r is r+6 */
        if (cumulative >= n) break;   /* found the correct row */
    }

    /* Previous cumulative count before row r */
    idx_t prev_cumulative = cumulative - (r + 6);
    /* Column index within row r */
    idx_t k = n - prev_cumulative - 1;

    seq_t res = 0;
    idx_t max_i = (k < 6 ? k : 6);
    for (idx_t i = 0; i <= max_i; ++i) {
        /* Skip terms where the binomial coefficient would be zero */
        if (k - i > r - 1) continue;
        seq_t v = binomial(r - 1, k - i);
        res += bin6[(size_t)i] * v;
    }
    return res;
}
