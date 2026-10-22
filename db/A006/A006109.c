/*
 * A006109  Gaussian binomial coefficient [ n,n/2 ] for q=4.
 * https://oeis.org/A006109
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [n, floor(n/2)]_4 = Product_{i=1..k}
 *            (4^(n-k+i)-1)/(4^i-1), k = floor(n/2); fixed-width builds use
 *            the q-Pascal rule [m,j] = [m-1,j-1] + 4^j*[m-1,j], [m,0] = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   alloc_zero, release, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/seq_is_fixed_width.h"

seq_t a(idx_t n) {
    idx_t k = n / 2;
    if (k == 0) return (seq_t)1;

    if (seq_is_fixed_width()) {
        /* q-Pascal triangle: every intermediate is <= [n,k], so no spurious overflow */
        seq_t *c = alloc_zero(k + 1);
        seq_t *pw = alloc_zero(k + 1);
        seq_t r;
        idx_t m, j;
        pw[0] = 1;
        for (j = 1; j <= k; j++) pw[j] = pw[j - 1] * 4;
        c[0] = 1;
        for (m = 1; m <= n; m++) {
            idx_t top = m < k ? m : k;
            for (j = top; j >= 1; j--)
                c[j] = c[j - 1] + pw[j] * c[j];
        }
        r = c[k];
        release(c, k + 1);
        release(pw, k + 1);
        return r;
    } else {
        /* partial product after step i equals [n-k+i, i]_4, so each division is exact */
        seq_t result = 1;
        seq_t pnum = 1, pden = 1;
        idx_t i;
        for (i = 0; i < n - k; i++) pnum *= 4;
        for (i = 1; i <= k; i++) {
            pnum *= 4;
            pden *= 4;
            result = result * (pnum - 1) / (pden - 1);
        }
        return result;
    }
}
