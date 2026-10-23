/*
 * A184532  Array, read by rows: T(n,h)=floor[1/{(h+n^3)^(1/3)}], where
 *          h=1,2,...,3n^2+3n and {}=fractional part.
 * https://oeis.org/A184532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is read by rows from T(n,h) = floor(1/frac( (h+n^3)^(1/3)
 *            )) where h = 1 .. 3n^2+3n. Offset starts at 1
 * Range:     u8 n <= 162, u16 n <= 162, u32 n <= 162, u64 n <= 162,
 *            u128 n <= 162, big any n
 * Helpers:   cube_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/cube_idx.h"

/* Small helper: integer cube root via binary search (floor).  */
static inline idx_t int_cuberoot(idx_t v) {
    idx_t lo = 0, hi = 1;
    while (cube_idx(hi) <= v) hi <<= 1;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (cube_idx(mid) <= v)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Main sequence function */
seq_t a(idx_t n) {
    /* Find the row r such that r*(r+1)*(r+2) >= n >
       (r-1)*r*(r+1) < n.  */
    idx_t r = int_cuberoot(n);
    /* Adjust r to satisfy r(r+1)(r+2) >= n */
    while (((unsigned __int128)r * (r + 1) * (r + 2)) < n) ++r;
    while (r > 0 &&
           ((unsigned __int128)(r - 1) * r * (r + 1)) >= n) --r;

    /* Offset of previous row */
    idx_t prev_offset = ((unsigned __int128)(r - 1) * r * (r + 1));
    /* Position within the row (1‑based) */
    idx_t h = n - prev_offset;
    /* x = r^3 + h   (always < (r+1)^3, so floor cube root is r) */
    seq_t x = (seq_t)cube_idx(r) + (seq_t)h;

    /* Binary search for m: largest m such that
       x * m^3 <= (r*m + 1)^3  */
    /* Upper bound for m is roughly 3*r^2 + 3*r + 1.  */
    idx_t lo = 1, hi = 3 * r * r + 3 * r + 1;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        unsigned __int128 left  = (unsigned __int128)x * mid * mid * mid;
        unsigned __int128 right = (unsigned __int128)(r * mid + 1);
        right = right * right * right;
        if (left <= right)
            lo = mid;
        else
            hi = mid - 1;
    }
    return (seq_t)lo;
}
