/*
 * A128177  A128174 * A004736 as infinite lower triangular matrices.
 * https://oeis.org/A128177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by flattening the infinite lower triangular
 *            matrix T(r, c) = A002620(r - c + 1), where A002620(m) =
 *            floor((m+1)^2/4). Thus for an index n (1‑based): find the unique
 *            r with r(r+1)/2 ≥ n, let k = n - r(r-1)/2 (1 ≤ k ≤ r), let m = r
 *            - k + 1, a(n) = floor((m+1)^2/4). This reproduces...
 * Range:     u8 n <= 44, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* locate row r: minimal r such that r*(r+1)/2 >= n */
    idx_t lo = 1, hi = n;   /* hi = n is safe upper bound */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        seq_t tri = (seq_t)mid * (mid + 1) / 2;      /* mid*(mid+1)/2 */
        if (tri >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t r = lo;   /* row index */

    /* previous total entries before row r */
    seq_t prev_total = (seq_t)(r - 1) * r / 2;
    idx_t k = (idx_t)((seq_t)n - prev_total);      /* column index (1‑based) */

    idx_t m = r - k + 1;                           /* argument to A002620 */

    seq_t mp1 = (seq_t)(m + 1);
    return mp1 * mp1 / 4;                          /* floor((m+1)^2/4) */
}
