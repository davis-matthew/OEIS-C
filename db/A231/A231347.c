/*
 * A231347  Triangle read by rows: T(n,k), n>=1, k>=1, in which column k lists
 *          the odd numbers interleaved with k-1 zeros but T(n,1) = n - 1 and
 *          the first element of column k is in row k(k+1)/2.
 * https://oeis.org/A231347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   Triangle T(n,k): T(n,1) = n-1 For k >= 2: let t = k(k+1)/2 if n
 *            < t -> column not present else d = n - t if d % k == 0 -> T(n,k)
 *            = 2*(d/k)+1 (odd number) else -> T(n,k) = 0 a(n) is the n-th
 *            element of the triangle read by rows (row 1: T(1,1), row 2:
 *            T(2,1), row 3: T(3,1),T(3,2), … )
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>        /* for size_t */

static inline idx_t triangle(idx_t k) {
    return (k * (k + 1)) / 2;
}

/* greatest k with k(k+1)/2 <= r */
static idx_t kmax_of_row(idx_t r)
{
    /* binary search */
    idx_t lo = 0, hi = 1;
    while (triangle(hi) <= r) hi <<= 1;
    while (lo + 1 < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (triangle(mid) <= r) lo = mid;
        else hi = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* find row r such that cumulative entries up to r >= n */
    idx_t r = 1;
    idx_t cum = 0;
    for (;;) {
        idx_t kmax = kmax_of_row(r);
        if (cum + kmax >= n) break;
        cum += kmax;
        ++r;
    }

    /* column index in this row (1‑based) */
    idx_t col = n - cum;
    if (col == 1)                         /* first column */
        return (seq_t)(r - 1);

    /* column k = col for k >= 2 */
    idx_t k = col;
    idx_t t_k = triangle(k);
    idx_t diff = r - t_k;                /* r >= t_k guaranteed */

    if (diff % k == 0) {
        idx_t p = diff / k;
        return (seq_t)(2 * p + 1);
    }
    return (seq_t)0;
}
