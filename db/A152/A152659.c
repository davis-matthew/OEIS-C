/*
 * A152659  Triangle read by rows: T(n,k) is the number of lattice paths from
 *          (0,0) to (n,n) with steps E=(1,0) and N=(0,1) and having k turns
 *          (NE or EN) (1<=k<=2n-1).
 * https://oeis.org/A152659
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = T(r, k) where * r = ceil(sqrt(n)) * k = n - (r-1)^2 (1 ≤
 *            k ≤ 2r-1) * if k is odd (k=2j-1): T = 2 * C(r-1, j-1)^2 * if k
 *            is even (k=2j ): T = 2 * C(r-1, j-1) * C(r-1, j) * * The
 *            function uses the helper binom(n, k) from ../utils/binom.h
 * Range:     u8 n <= 40, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"

/* Integer floor square root of a 64‑bit unsigned integer. */
static inline idx_t floor_sqrt(idx_t n)
{
    if (n < 2) return n;
    idx_t lo = 0, hi = n, ans = 0;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= n) {
            ans = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined but safe */

    /* determine the row r = ceil(sqrt(n)) */
    idx_t r = floor_sqrt(n - 1) + 1;

    /* previous row ends at (r-1)^2 */
    unsigned __int128 prev_sq_128 = (unsigned __int128)(r - 1) * (r - 1);
    idx_t prev_sq = (idx_t)prev_sq_128;

    /* position within the row */
    idx_t k = n - prev_sq;         /* 1 … 2r-1 */

    /* compute the binomial components */
    idx_t j;
    seq_t c1, c2, result;

    if (k & 1) {                   /* odd k: k = 2j-1 */
        j = (k + 1) >> 1;          /* j = (k+1)/2 */
        c1 = binom(r - 1, j - 1);
        result = 2 * c1 * c1;
    } else {                       /* even k: k = 2j   */
        j = k >> 1;                /* j = k/2         */
        c1 = binom(r - 1, j - 1);
        c2 = binom(r - 1, j);
        result = 2 * c1 * c2;
    }
    return result;
}
