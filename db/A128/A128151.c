/*
 * A128151  A002260 * A097806.
 * https://oeis.org/A128151
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Let r be the unique integer such that r(r+1)/2 ≥ n > (r−1)r/2
 *            (i.e., r is the row containing the n‑th element). Let pos = n −
 *            (r−1)r/2 (1 ≤ pos ≤ r). Then the sequence value is a(n) = (pos <
 *            r) ? (2*(pos+1) − 1) : r For n = 1 the value is 1
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)   /* offset 1 – n ≥ 1 expected, but guard anyway */
        return (seq_t)0;

    /* Find the row r by binary search on the triangular numbers. */
    idx_t lo = 1, hi = n;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t r = lo;

    /* Previous row total length.  Use 128–bit ops to avoid overflow. */
    idx_t prev = (unsigned __int128)(r - 1) * r / 2;

    /* Position of the element inside its row. */
    idx_t pos = n - prev;

    /* Compute the term. */
    if (r == 1)
        return (seq_t)1;
    if (pos < r)
        return (seq_t)(2 * (pos + 1) - 1);
    return (seq_t)r;
}
