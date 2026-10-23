/*
 * A115713  A divide-and-conquer related triangle.
 * https://oeis.org/A115713
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   a(n) is the nth value obtained by reading the infinite triangle
 *            T(r,k) row‑by‑row. For r ≥ 0, k = 0 … r the entry is T(r,k) = 1
 *            if k == r -4 if r == 2*k + 2 -(1 + (-1)^k)/2 if r == k + 1 0
 *            otherwise Thus a(n) corresponds to the pair (r,k) where r is the
 *            largest integer satisfying r(r+1)/2 ≤ n, an...
 * Range:     i8 n <= 1325, i16 n <= 1325, i32 n <= 1325, i64 n <= 1325,
 *            i128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t
tri_leq(idx_t n)          /* largest m with m(m+1)/2 ≤ n */
{
    idx_t lo = 0, hi = 1;
    /* exponential search for an upper bound */
    while ((unsigned __int128)hi * (hi + 1) / 2 <= n)
        hi <<= 1;

    while (lo + 1 < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        unsigned __int128 v = (unsigned __int128)mid * (mid + 1) / 2;
        if (v <= n)
            lo = mid;
        else
            hi = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* locate the (row, column) of the nth element */
    idx_t row = tri_leq(n);
    unsigned __int128 tri   = (unsigned __int128)row * (row + 1) / 2;
    idx_t   col = n - (idx_t)tri;

    /* compute T(row,col) */
    if (col == row) {
        return (seq_t)1;
    }
    if (row >= 2 && (((row - 2) & 1ULL) == 0)) {
        idx_t k4 = (row - 2) / 2;
        if (col == k4) {
            return (seq_t)-4;
        }
    }
    if (col == row - 1) {
        /* -(1 + (-1)^k)/2  with k = col */
        seq_t parity = ((col & 1ULL) == 0) ? (seq_t)1 : (seq_t)-1;
        return (seq_t)(-(1 + parity) / 2);
    }
    return (seq_t)0;
}
