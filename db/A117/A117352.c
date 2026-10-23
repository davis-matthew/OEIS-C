/*
 * A117352  Riordan array (1/(1-2x), x(1-2x)/(1-x)).
 * https://oeis.org/A117352
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   a(n) = T(r, k) where r = floor((sqrt(8n+1)-1)/2), k = n -
 *            r(r+1)/2, and T(r,k) = Σ_{j=0}^{r-k} C(j-k, j) · C(r-k, j) with
 *            the general binomial C(α,β) defined for negative α by C(α,β) =
 *            (-1)^β C(β-α-1, β)
 * Range:     i8 n <= 27, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* find the row r such that r(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = (low + high + 1) / 2;
        __int128 tri = (__int128)mid * (mid + 1) / 2;
        if (tri <= n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t r = low;
    __int128 tri_r = (__int128)r * (r + 1) / 2;
    idx_t k = n - (idx_t)tri_r;          /* column index within the row */
    idx_t m = r - k;                     /* upper limit for the inner sum */

    seq_t sum = 0;

    for (idx_t j = 0; j <= m; ++j) {
        /* compute C(j-k, j) with general binomial for negative upper index */
        int64_t diff = (int64_t)j - (int64_t)k;
        seq_t bin1;
        if (diff >= 0) {
            bin1 = binom((idx_t)diff, j);
        } else {
            int64_t mneg = -diff;                      /* -diff = k-j > 0 */
            idx_t top = (idx_t)(j + mneg - 1);          /* j + (k-j) - 1 = k-1 */
            seq_t tmp = binom(top, j);
            if (j & 1)
                bin1 = -tmp;
            else
                bin1 = tmp;
        }

        /* compute ordinary binomial C(r-k, j) */
        seq_t bin2 = binom(m, j);

        sum += bin1 * bin2;
    }

    return sum;
}
