/*
 * A006330  Number of corners, or planar partitions of n with only one row and
 *          one column.
 * https://oeis.org/A006330
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f.: (Sum_{k>=0} (-1)^k * x^(k(k+1)/2)) / (Product_{k>0} (1 -
 *            x^k))^2, i.e. a(n) = Sum_{k>=0} (-1)^k * A000712(n - k(k+1)/2),
 *            where A000712 is computed by the pentagonal-number recurrence
 *            applied twice
 * Range:     u8 n <= 9, u16 n <= 24, u32 n <= 70, u64 n <= 221, u128 n <= 758,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/alloc_zero.h"
#include "../utils/release.h"

seq_t a(idx_t n) {
    idx_t len = n + 1;
    seq_t *g = alloc_zero(len); /* 1/(x;x)_inf      : A000041 */
    seq_t *h = alloc_zero(len); /* 1/(x;x)_inf^2    : A000712 */

    for (idx_t m = 0; m <= n; m++) {
        seq_t pos = 0, neg = 0;
        if (m == 0) pos = 1;
        for (idx_t k = 1;; k++) {
            idx_t p1 = k * (3 * k - 1) / 2;
            if (p1 > m) break;
            idx_t p2 = k * (3 * k + 1) / 2;
            if (k & 1) {
                pos += g[m - p1];
                if (p2 <= m) pos += g[m - p2];
            } else {
                neg += g[m - p1];
                if (p2 <= m) neg += g[m - p2];
            }
        }
        g[m] = pos - neg;

        pos = g[m];
        neg = 0;
        for (idx_t k = 1;; k++) {
            idx_t p1 = k * (3 * k - 1) / 2;
            if (p1 > m) break;
            idx_t p2 = k * (3 * k + 1) / 2;
            if (k & 1) {
                pos += h[m - p1];
                if (p2 <= m) pos += h[m - p2];
            } else {
                neg += h[m - p1];
                if (p2 <= m) neg += h[m - p2];
            }
        }
        h[m] = pos - neg;
    }

    seq_t pos = 0, neg = 0;
    for (idx_t k = 0; k * (k + 1) / 2 <= n; k++) {
        idx_t t = k * (k + 1) / 2;
        if (k & 1) neg += h[n - t];
        else pos += h[n - t];
    }
    seq_t result = pos - neg;

    release(g, len);
    release(h, len);
    return result;
}
