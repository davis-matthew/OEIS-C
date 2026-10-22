/*
 * A001679  Number of series-reduced rooted trees with n nodes.
 * https://oeis.org/A001679
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f. = 1 + ((1+x)*f(x) - (f(x)^2+f(x^2))/2)/x, f(x) = x*L(x) =
 *            g.f. of A001678, where L(x) = x*(E(x) - L(x)), E(x) =
 *            Prod_{k>=1} 1/(1-x^k)^L(k) (Euler transform of L); L(1) = 1,
 *            L(m+1) = E(m) - L(m). E is computed via n*E(n) = Sum_{k=1..n}
 *            c(k)*E(n-k), c(k) = Sum_{d|k} d*L(d) (arbitrary precision...
 * Range:     u8 n <= 12, u16 n <= 21, u32 n <= 36, u64 n <= 65, u128 n <= 123,
 *            big any n
 * Helpers:   a1679_bits, a1679_final, a1679_gcd, a1679_mul, a1679_tri,
 *            alloc_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/alloc_array.h"
#include "../utils/a1679_bits.h"
#include "../utils/a1679_final.h"
#include "../utils/a1679_gcd.h"
#include "../utils/a1679_mul.h"
#include "../utils/a1679_tri.h"

static seq_t *bL = NULL, *bE = NULL, *bC = NULL;
static idx_t bcap = 0;

seq_t a(idx_t n) {
    int bits;
    if (n == 0) return 1;
    bits = a1679_bits();
    if (bits > 0) {
        idx_t N, m, k, j, W;
        seq_t *L, *F, res;
        if (n > (idx_t)(2 * bits + 20)) return 0;
        N = n + 1;
        W = N + 1;
        L = (seq_t *)malloc((size_t)W * sizeof(seq_t));
        F = (seq_t *)malloc((size_t)W * (size_t)W * sizeof(seq_t));
        for (k = 0; k <= N; k++) { L[k] = 0; F[k] = 1; }
        L[1] = 1;
        for (m = 1; m + 1 <= n; m++) {
            F[m * W] = 0;
            for (k = 1; k <= m; k++) {
                seq_t v = F[m * W + k - 1];
                seq_t r = 1;
                for (j = 1; j * k <= m; j++) {
                    seq_t num = L[k] + (seq_t)(j - 1);
                    seq_t js = (seq_t)j;
                    seq_t g = a1679_gcd(r, js);
                    idx_t rem = m - j * k;
                    idx_t kk = (k - 1 < rem) ? k - 1 : rem;
                    r = a1679_mul(r / g, num / (js / g));
                    v += a1679_mul(r, F[rem * W + kk]);
                }
                F[m * W + k] = v;
            }
            L[m + 1] = (seq_t)(F[m * W + m] - L[m]);
        }
        res = a1679_final(L, n);
        free(L);
        free(F);
        return res;
    }
    if (n + 1 > bcap) {
        idx_t cap = n + 1, m, k;
        if (cap < 2 * bcap) cap = 2 * bcap;
        if (cap < 64) cap = 64;
        bL = alloc_array(cap + 1);
        bE = alloc_array(cap + 1);
        bC = alloc_array(cap + 1);
        for (k = 0; k <= cap; k++) { bL[k] = 0; bE[k] = 0; bC[k] = 0; }
        bE[0] = 1;
        bL[1] = 1;
        for (m = 1; m < cap; m++) {
            seq_t s = 0;
            if (m >= 2) bL[m] = bE[m - 1] - bL[m - 1];
            for (k = m; k < cap; k += m) bC[k] += bL[m] * (seq_t)m;
            for (k = 1; k <= m; k++) s += bC[k] * bE[m - k];
            bE[m] = s / (seq_t)m;
        }
        bcap = cap;
    }
    return a1679_final(bL, n);
}
