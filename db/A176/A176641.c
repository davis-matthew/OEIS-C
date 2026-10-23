/*
 * A176641  Triangle T(n, k) = 28^(k*(n-k)), read by rows.
 * https://oeis.org/A176641
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 28^(k*(r-k)) where r is the row index satisfying r(r+1)/2
 *            ≤ n < (r+1)(r+2)/2 and k = n – r(r+1)/2 is the column index
 *            within that row
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 38, u128 n <= 69,
 *            big any n
 * Helpers:   pow28
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow28.h"

seq_t a(idx_t n)
{
    /* Find the row r for the linear index n.
       Solve r(r+1)/2 ≤ n < (r+1)(r+2)/2. */
    idx_t r, start;

    {
        /* Using integer arithmetic only on idx_t. */
        idx_t disc   = 8 * n + 1;
        /* Integer square root of disc. */
        idx_t sq = (idx_t)0;
        {
            /* Binary search for floor(sqrt(disc)). */
            idx_t lo = 0, hi = disc;
            while (lo <= hi) {
                idx_t mid = (lo + hi) >> 1;
                idx_t sq_mid = mid * mid;
                if (sq_mid == disc) {
                    sq = mid;
                    break;
                } else if (sq_mid < disc) {
                    lo = mid + 1;
                    sq = mid;
                } else {
                    hi = mid - 1;
                }
            }
        }
        r = (sq - 1) >> 1;
    }

    start = r * (r + 1) / 2;
    idx_t k = n - start;
    idx_t exp = k * (r - k);   /* exponent of 28 */

    return pow28(exp);
}
