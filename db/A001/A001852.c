/*
 * A001852  Total diameter of labeled trees with n nodes.
 * https://oeis.org/A001852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d} d*T(n,d), T(n,d) = A034854(n,d) = number of
 *            labeled trees on n nodes with diameter d, computed from the
 *            rooted height-limited EGFs R_{-1}=0, R_0 = x, R_h =
 *            x*exp(R_{h-1}): T(n,2h) = n! [x^n] x*(exp(R_{h-1}) -
 *            exp(R_{h-2})*(1 + R_{h-1} - R_{h-2})) (h>=1) T(n,2h+1) = n!
 *            [x^n] (R_h -...
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   a001852_arr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/a001852_arr.h"

seq_t a(idx_t n)
{
    idx_t m, k, h, N;
    seq_t total = 0;
    seq_t *C, *cur, *prev, *prev2, *Ecur, *Eprev, *Eprev2, *t;

    if (n <= 1) return 0;
    N = n + 1;

    /* Pascal triangle C(m,k), 0<=k<=m<=n */
    C = a001852_arr(N * (N + 1) / 2);
#define BIN(mm, kk) C[(mm) * ((mm) + 1) / 2 + (kk)]
    for (m = 0; m <= n; m++) {
        BIN(m, 0) = 1;
        BIN(m, m) = 1;
        for (k = 1; k < m; k++) BIN(m, k) = BIN(m - 1, k - 1) + BIN(m - 1, k);
    }

    cur = a001852_arr(N);
    prev = a001852_arr(N);
    prev2 = a001852_arr(N);
    Ecur = a001852_arr(N);
    Eprev = a001852_arr(N);
    Eprev2 = a001852_arr(N);

    /* h = 0: cur = x, prev = prev2 = 0 */
    cur[1] = 1;
    for (m = 0; m <= n; m++) Ecur[m] = 1;   /* exp(x) */
    Eprev[0] = 1;
    Eprev2[0] = 1;

    for (h = 0;; h++) {
        /* odd diameter 2h+1 */
        if (2 * h + 1 <= n - 1) {
            seq_t s = 0;
            for (k = 1; k < n - k; k++) {
                seq_t dk = cur[k] - prev[k];
                seq_t dn = cur[n - k] - prev[n - k];
                s += BIN(n, k) * dk * dn;
            }
            if (n % 2 == 0) {
                seq_t dh = cur[n / 2] - prev[n / 2];
                s += BIN(n - 1, n / 2 - 1) * dh * dh;
            }
            total += (seq_t)(2 * h + 1) * s;
        }
        /* even diameter 2h, h >= 1: A = prev, B = prev2 */
        if (h >= 1 && 2 * h <= n - 1) {
            idx_t mm = n - 1;
            seq_t val = Eprev[mm] - Eprev2[mm];
            for (k = 1; k <= mm; k++) {
                seq_t d = prev[k] - prev2[k];
                if (d != 0 && Eprev2[mm - k] != 0)
                    val -= BIN(mm, k) * d * Eprev2[mm - k];
            }
            total += (seq_t)(2 * h) * (seq_t)n * val;
        }
        if (2 * h > n - 1) break;

        /* advance: new cur = R_{h+1} = x*exp(R_h) */
        t = prev2; prev2 = prev; prev = cur; cur = t;
        t = Eprev2; Eprev2 = Eprev; Eprev = Ecur; Ecur = t;
        cur[0] = 0;
        for (m = 1; m <= n; m++) cur[m] = (seq_t)m * Eprev[m - 1];
        Ecur[0] = 1;
        for (m = 1; m <= n; m++) {
            seq_t s = 0;
            for (k = 1; k <= m; k++)
                s += BIN(m - 1, k - 1) * cur[k] * Ecur[m - k];
            Ecur[m] = s;
        }
    }
#undef BIN

    free(C);
    free(cur); free(prev); free(prev2);
    free(Ecur); free(Eprev); free(Eprev2);
    return total;
}
