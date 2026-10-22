/*
 * A006115  Gaussian binomial coefficient [ n,n/2 ] for q=5.
 * https://oeis.org/A006115
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [n, floor(n/2)]_5, computed with the q-Pascal recurrence
 *            [m,j] = [m-1,j-1] + 5^j*[m-1,j], [m,0] = 1, [0,j] = 0 for j > 0
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/alloc_zero.h"
#include "../utils/release.h"

seq_t a(idx_t n) {
    idx_t k = n / 2;
    if (k == 0) return 1;

    seq_t *c = alloc_zero(k + 1);
    seq_t *p = alloc_zero(k + 1);
    idx_t j, m;

    p[0] = 1;
    for (j = 1; j <= k; j++) p[j] = p[j - 1] * 5;

    c[0] = 1;
    for (m = 1; m <= n; m++) {
        idx_t top = m < k ? m : k;
        for (j = top; j >= 1; j--) {
            c[j] = c[j - 1] + p[j] * c[j];
        }
    }

    seq_t result = c[k];
    release(c, k + 1);
    release(p, k + 1);
    return result;
}
