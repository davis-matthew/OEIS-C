/*
 * A103881  Square array T(n,k) (n >= 1, k >= 0) read by antidiagonals:
 *          coordination sequence for root lattice A_n.
 * https://oeis.org/A103881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r, c) where T(r, c) = ∑_{i=1..r} C(r+1, i)·C(c‑1,
 *            i‑1)·C(r‑i+c, c) with T(r, 0) = 1. The index n is read by
 *            antidiagonals: for each m ≥ 0, list T(m+1,0), T(m,1), …, T(1,m).
 *            The mapping from n to (r,c) is described in the function body
 * Range:     u8 n <= 24, u16 n <= 70, u32 n <= 239, u64 n <= 835,
 *            u128 n <= 2959, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* indices below the offset are irrelevant */
    /* locate the antidiagonal m such that
       S(m) = m*(m+1)/2  < n ≤ S(m+1) = (m+1)*(m+2)/2 */
    idx_t m = 0;
    idx_t lo = 0, hi = n;               /* hi is an upper bound */
    while (lo <= hi) {
        idx_t mid = (lo + hi) / 2;
        idx_t s = mid * (mid + 1) / 2;   /* S(mid) */
        if (s < n) {
            m = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    /* starting position of this antidiagonal */
    idx_t start = m * (m + 1) / 2 + 1;
    idx_t offset = n - start;           /* 0 ≤ offset ≤ m */
    idx_t row = m + 1 - offset;         /* r ≥ 1 */
    idx_t col = offset;                 /* c ≥ 0 */

    if (col == 0)
        return 1;                      /* T(r, 0) = 1 */

    /* compute T(row, col) by the defining sum */
    seq_t sum = 0;
    for (idx_t i = 1; i <= row; ++i) {
        seq_t term = binom(row + 1, i);
        term = term * binom(col - 1, i - 1);
        term = term * binom(row - i + col, col);
        sum += term;
    }
    return sum;
}
