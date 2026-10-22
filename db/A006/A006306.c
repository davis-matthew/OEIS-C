/*
 * A006306  Coefficients of the '2nd-order' mock theta function mu(q).
 * https://oeis.org/A006306
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, nice
 * Formula:   G.f.: Sum_{k >= 0} (-1)^k q^(k^2) (1-q)(1-q^3)...(1-q^(2k-1)) /
 *            ((1+q^2)^2 (1+q^4)^2 ... (1+q^(2k))^2)
 * Range:     i8 n <= 42, i16 n <= 144, i32 n <= 508, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    idx_t len = n + 1;
    seq_t *p = (seq_t *)calloc(len, sizeof(seq_t));
    seq_t ans = 0;
    idx_t k, i;
    p[0] = 1;
    for (k = 0; k * k <= n; k++) {
        idx_t m = n - k * k; /* need p up to degree m */
        if (k > 0) {
            idx_t e = 2 * k - 1;
            /* multiply by (1 - q^e) */
            for (i = m; i >= e; i--) {
                p[i] -= p[i - e];
                if (i == e) break;
            }
            e = 2 * k;
            /* divide by (1 + q^e) twice */
            for (int t = 0; t < 2; t++)
                for (i = e; i <= m; i++)
                    p[i] -= p[i - e];
        }
        if (k % 2 == 0) ans += p[m];
        else ans -= p[m];
    }
    free(p);
    return ans;
}
