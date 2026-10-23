/*
 * A049801  Triangular array T, read by rows: T(n,k) = n mod floor(k/3), k =
 *          3..n and n >= 3.
 * https://oeis.org/A049801
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by reading the triangular array T(n,k) = n mod
 *            floor(k/3) with n ≥ 3, k = 3…n, row by row starting at n = 3.
 *            a(3) is the first element
 * Range:     u8 n <= 4853, u16 n <= 4853, u32 n <= 4853, u64 n <= 4853,
 *            u128 n <= 4853, big any n
 * Helpers:   row_last128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/row_last128.h"

seq_t a(idx_t n)
{
    if (n < 3) return 0;                    /* offset starts at 3 */

    /* find the row r that contains index n */
    idx_t lo = 3, hi = 3;
    /* enlarge hi until it covers n */
    while (row_last128(hi) < n) {
        if (hi > (idx_t)(-1) / 2) break;    /* avoid overflow */
        hi *= 2;
    }
    /* binary search in [lo,hi] */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (row_last128(mid) >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t r = lo;                           /* the row number */

    /* compute start index of row r */
    idx_t start = 3 + (idx_t)(((__int128)(r - 3) * (r - 2)) / 2);

    /* position within the row */
    idx_t offset = n - start;                /* 0‑based */
    idx_t k = 3 + offset;                    /* column index */

    idx_t d = k / 3;                         /* floor(k/3), >0 */
    return (seq_t)(r % d);                   /* T(r,k) */
}
