/*
 * A193592  Triangle read by rows having n-th row 1, n, n-1, n-2,..., 2, 1 for
 *          n>=0.
 * https://oeis.org/A193592
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = n-th term of the triangle read by rows where row r has
 *            the entries 1, r, r-1, ..., 2, 1. The function locates the row
 *            that contains index n, then returns the element at that position
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>  /* for bool */

static idx_t _isqrt_u128(__int128 x) {
    idx_t low = 0;
    idx_t high = (idx_t)1 << 34;           /* > sqrt(8*(2^64-1)+1) */
    idx_t ans  = 0;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        __int128 sq = (__int128)mid * mid;
        if (sq <= x) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    __int128 t = (__int128)8 * n + 1;
    idx_t sqrt_val = _isqrt_u128(t);        /* floor(sqrt(8*n+1)) */
    idx_t r = (sqrt_val - 1) / 2;

    idx_t start = (idx_t)(((__int128)r * (r + 1)) / 2);
    idx_t k = n - start;                    /* position inside the row */

    if (k == 0 || k == r) {
        return (seq_t)1;
    } else {
        return (seq_t)(r - k + 1);
    }
}
