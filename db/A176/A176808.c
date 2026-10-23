/*
 * A176808  Triangle read by rows: T(n,m)=Floor[(n - 1)/m], 1<=m<=n.
 * https://oeis.org/A176808
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = floor((r-1)/m), where r is the smallest integer with
 *            r(r+1)/2 ≥ n and m = n - r(r-1)/2
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* seq_t and idx_t are supplied by the build system. */
seq_t a(idx_t n)
{
    /* n ≥ 1 (offset 1). */
    /* Binary search for the row r. */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;                           /* row index */
    unsigned __int128 prev_tri = (unsigned __int128)(r - 1) * r / 2;
    idx_t m = n - (idx_t)prev_tri;           /* column index (1‑based) */

    /* Value in the triangle. */
    idx_t q = (r - 1) / m;                   /* floor division */
    return (seq_t)q;
}
