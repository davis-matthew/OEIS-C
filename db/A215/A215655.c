/*
 * A215655  Irregular triangle read by rows: reading the n-th row describes
 *          all the numbers seen in the triangle up to the end of the n-th
 *          row.
 * https://oeis.org/A215655
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   Let r be the smallest integer with r*(r+1) ≥ n. Then the row r
 *            contributes 2r terms. Let offset = n - r*(r-1) - 1 (0‑based
 *            within the row). If offset is even, the term equals 2 * (r -
 *            (offset/2)) // the count for the pair otherwise it equals 2 *
 *            (offset/2 + 1) // the value for the pair Initial va...
 * Range:     u8 n <= 110, u16 n <= 110, u32 n <= 110, u64 n <= 110,
 *            u128 n <= 110, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* find r: r*(r+1) >= n */
    idx_t lo = 1, hi = n;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        unsigned __int128 prod = (unsigned __int128)mid * (mid + 1);
        if (prod >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t r = lo;
    unsigned __int128 before = (unsigned __int128)(r - 1) * r;   /* terms before row r */
    const idx_t offset = (idx_t)(n - before - 1);                /* 0‑based in row */

    if ((offset & 1) == 0) { /* count part */
        idx_t i = offset / 2 + 1;           /* 1‑based pair index */
        seq_t val = (seq_t)(2 * (r - i + 1));
        return val;
    } else {                      /* value part */
        idx_t i = offset / 2 + 1;
        seq_t val = (seq_t)(2 * i);
        return val;
    }
}
