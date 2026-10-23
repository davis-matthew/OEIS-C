/*
 * A123961  Triangle T(n, k) = k^2*(1+n)^2 - 4*n, read by rows.
 * https://oeis.org/A123961
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, sign
 * Formula:   a(n) = T(r, k) where T(r,k) = k^2*(1+r)^2 - 4*r, r =
 *            floor((sqrt(8n+1)-1)/2) (row index), k = n - r*(r+1)/2 (column
 *            index)
 * Range:     i8 n <= 8, i16 n <= 103, i32 n <= 5150, i64 n <= 5150,
 *            i128 n <= 5150, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* find r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t low = 0, high = n, r = 0;
    while (low <= high)
    {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 sum = (unsigned __int128)mid * (mid + 1) / 2;
        if (sum <= n)
        {
            r = mid;
            low = mid + 1;
        }
        else
        {
            if (mid == 0) break;  /* avoid underflow */
            high = mid - 1;
        }
    }

    /* index of the first element of row r */
    unsigned __int128 start128 = (unsigned __int128)r * (r + 1) / 2;
    idx_t start = (idx_t)start128;
    idx_t k = n - start;           /* column index within the row */

    seq_t rt = (seq_t)r;
    seq_t kt = (seq_t)k;
    seq_t term = kt * kt * (rt + 1) * (rt + 1) - (seq_t)4 * rt;
    return term;
}
