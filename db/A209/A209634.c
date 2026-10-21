/*
 * A209634  Triangle with (1,4,7,10,13,16...,(3*n-2),...) in every column,
 *          shifted down twice.
 * https://oeis.org/A209634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   a(n) = 3*r - 6*k + 4 where (r,k) are the row and column indices
 *            of the triangle T(r,k) = 3*r - 6*k + 4. The row r is obtained
 *            from n by locating the triangle diagonal that contains the n‑th
 *            element
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t floor_isqrt_u64(idx_t n)
{
    idx_t low = 0, high = (idx_t)-1, ans = 0;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= n) {
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
    if (n == 0) return 0;          /* undefined for n==0; return 0 */
    idx_t m = floor_isqrt_u64(n);
    idx_t r, prev;

    if (n <= m * m) {               /* odd row 2*m-1 */
        r = 2 * m - 1;
        prev = (m - 1) * m;
    } else if (n <= m * (m + 1)) {  /* even row 2*m */
        r = 2 * m;
        prev = m * m;
    } else {                       /* odd row with m+1 */
        m = m + 1;
        r = 2 * m - 1;
        prev = (m - 1) * m;
    }

    idx_t k = n - prev;             /* 1‑based column index */
    seq_t ans = (seq_t)3 * (seq_t)r - (seq_t)6 * (seq_t)k + (seq_t)4;
    return ans;
}
