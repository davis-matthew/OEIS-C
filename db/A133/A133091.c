/*
 * A133091  A133080 * A002260.
 * https://oeis.org/A133091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = row value in the triangle where odd rows contain 1,2,…,n
 *            and even rows contain 2,4,…,2(n‑1),n. The row r containing n is
 *            the smallest r with r(r+1)/2 >= n
 * Range:     u8 n <= 44, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"  /* for floor_sqrt(seq_t x) if needed */

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined, but guard */
    /* Find the row r such that r(r+1)/2 >= n.  Use binary search. */
    idx_t lo = 1;
    idx_t hi = n;
    idx_t r = n;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        seq_t tri = (seq_t)mid * (mid + 1) / 2;
        if (tri >= n) {
            r = mid;
            hi = mid - 1;
        } else
            lo = mid + 1;
    }

    /* Compute the offset of the first element of row r */
    seq_t offset = (seq_t)(r - 1) * r / 2;
    idx_t k = n - (idx_t)offset;      /* position in the row (1‑based) */

    if (r & 1U)                         /* odd row: simple sequence */
        return (seq_t)k;
    else {                              /* even row */
        if (k < r)                      /* first r‑1 positions */
            return (seq_t)(2 * k);
        else                            /* last position = n */
            return (seq_t)r;
    }
}
