/*
 * A176890  Triangle T(n,k) read by rows. Signed subsequence of A051731.
 * https://oeis.org/A176890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = T(r,c) where r and c satisfy * r(r-1)/2 < n <= r(r+1)/2,
 *            c = n - r(r-1)/2. * T(r,c) = -1 if r=1 and c=1, * 0 if r = c, *
 *            1 if r mod c = 0, * 0 otherwise
 * Range:     i8 n <= 105, i16 n <= 105, i32 n <= 105, i64 n <= 105,
 *            i128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* find the row r by binary search on r ∈ [1..n] */
    idx_t lo = 1;
    idx_t hi = n;                /* r never exceeds n */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t r = lo;                /* row index */

    /* previous triangular number to locate the column */
    unsigned __int128 tri_prev = (unsigned __int128)(r - 1) * r / 2;
    idx_t c = n - (idx_t)tri_prev;   /* column index in this row */

    /* compute triangle entry */
    if (r == 1 && c == 1)
        return (seq_t)-1;
    if (r == c)
        return (seq_t)0;
    if (r % c == 0)
        return (seq_t)1;
    return (seq_t)0;
}
