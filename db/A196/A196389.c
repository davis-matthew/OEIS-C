/*
 * A196389  Triangle T(n,k), read by rows, given by (0,1,-1,0,0,0,0,0,0,0,...)
 *          DELTA (1,0,0,1,0,0,0,0,0,0,0,...) where DELTA is the operator
 *          defined in A084938.
 * https://oeis.org/A196389
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = 1 if n is the last element of a row, * a(n) = r-1 if n is
 *            the second‑to‑last element of row r, * otherwise 0, where r is
 *            the row index such that * r(r+1)/2 <= n < (r+1)(r+2)/2
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for unsigned 64‑bit integers. */
static idx_t integer_sqrt(idx_t x)
{
    idx_t low = 0, high = x, ans = 0;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= x) {
            ans = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    /* Find row r such that r(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t r = (integer_sqrt(8 * n + 1) - 1) / 2;

    /* Offset of first element of row r */
    idx_t offset = (unsigned __int128)r * (r + 1) / 2;
    idx_t pos = n - offset;               /* 0‑based position within the row */

    if (pos == r)           /* last element of the row */
        return (seq_t)1;
    if (r > 0 && pos == r - 1) /* second‑to‑last element */
        return (seq_t)(r - 1);
    return 0;
}
