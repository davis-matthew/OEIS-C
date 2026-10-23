/*
 * A176079  Triangle T(n,k) read by rows: If k divides n then k-1, otherwise
 *          -1.
 * https://oeis.org/A176079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = (k-1) if k divides r, otherwise -1 where r = smallest
 *            integer with r(r+1)/2 ≥ n k = n - r(r-1)/2 (column index in row
 *            r)
 * Range:     i8 n <= 5050, i16 n <= 5050, i32 n <= 5050, i64 n <= 5050,
 *            i128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1, but guard against n==0 */
        return 0;

    /* Find the row r such that the cumulative number of elements
       up to row r-1 is < n ≤ cumulative up to row r. */
    idx_t r;
    {
        idx_t lo = 1, hi = 1;
        /* Grow hi until the triangular number exceeds n */
        while (((unsigned __int128)hi * (hi + 1)) / 2 < n)
            hi <<= 1;          /* hi *= 2 */

        /* Binary search for the smallest r with triangular(r) ≥ n */
        while (lo < hi) {
            idx_t mid = lo + (hi - lo) / 2;
            unsigned __int128 t = ((unsigned __int128)mid * (mid + 1)) / 2;
            if (t >= n)
                hi = mid;
            else
                lo = mid + 1;
        }
        r = lo;
    }

    /* Compute the column index k inside this row */
    idx_t prev_tri = ((unsigned __int128)(r - 1) * r) / 2;
    idx_t k = n - prev_tri;            /* 1‑based column index */

    /* Return the triangle entry */
    if (r % k == 0)
        return (seq_t)(k - 1);
    else
        return (seq_t)(-1);
}
