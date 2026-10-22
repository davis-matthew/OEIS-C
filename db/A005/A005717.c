/*
 * A005717  Construct triangle in which n-th row is obtained by expanding (1 +
 *          x + x^2)^n and take the next-to-central column.
 * https://oeis.org/A005717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (A002426(n+1) - A002426(n))/2, where t(n) = A002426(n)
 *            (central trinomial coefficients) satisfies n*t(n) =
 *            (2n-1)*t(n-1) + 3(n-1)*t(n-2), t(0) = 1, t(1) = 1. For
 *            fixed-width seq_t, a(n) = T(n, n-1) is computed instead by
 *            adding up rows of the (1+x+x^2)^n triangle. That uses no
 *            division, so...
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 42, u128 n <= 83,
 *            big any n
 * Helpers:   seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_is_fixed_width.h"

seq_t a(idx_t n) {
    if (seq_is_fixed_width()) {
        /* coefficients 0..n-1 of (1+x+x^2)^r, built row by row */
        idx_t len = n;
        seq_t *row = (seq_t *)malloc(sizeof(seq_t) * (size_t)len);
        idx_t i, r;
        seq_t res;
        for (i = 0; i < len; i++) row[i] = 0;
        row[0] = 1;
        for (r = 1; r <= n; r++) {
            idx_t top = 2 * r;
            if (top > len - 1) top = len - 1;
            for (i = top + 1; i-- > 0;) {
                seq_t v = row[i];
                if (i >= 1) v += row[i - 1];
                if (i >= 2) v += row[i - 2];
                row[i] = v;
            }
        }
        res = row[n - 1];
        free(row);
        return res;
    } else {
        seq_t t0 = 1, t1 = 1; /* t(k-2), t(k-1) */
        seq_t tn = 1, tn1 = 1;
        idx_t k;
        for (k = 2; k <= n + 1; k++) {
            seq_t t2 = ((seq_t)(2 * k - 1) * t1 + (seq_t)(3 * (k - 1)) * t0) / (seq_t)k;
            t0 = t1;
            t1 = t2;
            if (k == n) tn = t2;
            if (k == n + 1) tn1 = t2;
        }
        if (n == 1) tn = 1; /* t(1) = 1 */
        return (tn1 - tn) / 2;
    }
}
