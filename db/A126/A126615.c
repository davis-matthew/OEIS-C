/*
 * A126615  Denominators in a harmonic triangle.
 * https://oeis.org/A126615
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, frac
 * Formula:   a(n) = k*(k+1) for k < r, otherwise a(n) = r where r is the row
 *            number such that (r-1)*r/2 < n ≤ r*(r+1)/2, and k = n -
 *            (r-1)*r/2 is the column index in the row
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* idx_t and seq_t are defined by the build */

static inline idx_t find_row(idx_t n)
{
    /* Binary search for the smallest r with r*(r+1)/2 >= n */
    idx_t lo = 0, hi = 1;
    unsigned __int128 tri;
    /* Exponential increase to find an upper bound */
    while (1) {
        tri = (unsigned __int128)hi * (hi + 1) / 2;
        if (tri >= n) break;
        hi <<= 1;          /* hi will never overflow for n ≤ 2^64-1 */
    }
    lo = hi >> 1;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri >= n) hi = mid;
        else          lo = mid + 1;
    }
    return hi;
}

seq_t a(idx_t n)
{
    if (n <= 1) return 1;  /* a(1) = 1 */

    idx_t r = find_row(n);
    /* previous triangular number: (r-1)*r/2 */
    unsigned __int128 prev_tri128 = (unsigned __int128)(r - 1) * r / 2;
    idx_t prev_tri = (idx_t)prev_tri128;
    idx_t k = n - prev_tri;           /* column index, 1‑based */

    if (k < r) {
        /* k*(k+1) fits in seq_t when it fits in the underlying type */
        return (seq_t)k * ((seq_t)k + 1);
    } else {
        /* last element of the row */
        return (seq_t)r;
    }
}
