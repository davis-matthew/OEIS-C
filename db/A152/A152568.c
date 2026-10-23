/*
 * A152568  Triangle T(n,k) read by rows: T(n,n) = -1, T(n,0) = 2^(n - 1),
 *          T(n,k) = -2^(n - k - 1), 1 <= k <= n - 1.
 * https://oeis.org/A152568
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, easy
 * Formula:   a(n) is obtained by reading the triangle T(n,k) row‑wise: *
 *            T(n,n) = -1, * T(n,0) = 2^(n-1), * T(n,k) = -2^(n-k-1) for 1 <=
 *            k <= n-1, * with row 0 consisting of the single entry -1. The
 *            OEIS index n * (offset 0) corresponds to the linear position in
 *            this triangle
 * Range:     i8 n <= 35, i16 n <= 63, i32 n <= 63, i64 n <= 63, i128 n <= 63,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

/* Find the row r such that r(r+1)/2 <= n < (r+1)(r+2)/2.
 * Use a binary search on idx_t; intermediate products are done in
 * __int128 to avoid overflow. */
static inline idx_t find_row(idx_t n)
{
    idx_t low  = 0;
    idx_t high = n + 1;          /* ensures (high)(high+1)/2 > n */
    while (low + 1 < high)
    {
        idx_t mid = low + (high - low) / 2;
        __int128 tri = (__int128)mid * (mid + 1) / 2;
        if (tri <= n)
            low = mid;
        else
            high = mid;
    }
    return low;
}

seq_t a(idx_t n)
{
    idx_t r = find_row(n);

    /* Row 0 is a special case: T(0,0) = -1. */
    if (r == 0)
        return -(seq_t)1;

    /* Number of elements before this row. */
    __int128 tri = (__int128)r * (r + 1) / 2;
    idx_t before = (idx_t)tri;

    idx_t col = n - before;     /* 0 <= col <= r */

    if (col == 0)               /* first element of the row */
        return pow2(r - 1);     /* 2^(r-1) */

    if (col == r)               /* last element of the row */
        return -(seq_t)1;       /* -1 */

    /* middle elements: negative powers of two */
    idx_t exp = r - col - 1;
    return -(pow2(exp));
}
