/*
 * A004533  Theta series of 12-dimensional unimodular lattice {D_12}^{+}.
 * https://oeis.org/A004533
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [q^n] (theta2(q)^12 + theta3(q)^12 + theta4(q)^12)/2 = (n
 *            even ? [q^n] theta3^12 : 0) + 2048*[q^(n-3)] (Sum_{k>=0}
 *            q^(k(k+1)))^12, using theta4^12 coefficient = (-1)^n * theta3^12
 *            coefficient and theta2^12 = 4096 q^3 (Sum_{k>=0} q^(k(k+1)))^12.
 *            All coefficients nonnegative
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 56, u64 n <= 4673,
 *            u128 n <= 10000, big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

/* [q^m] f^12, where f = 1 + w * Sum_{k>=1} q^(e(k)),
   e(k) = k^2, w = 2 (theta3) if tri == 0; e(k) = k(k+1), w = 1 if tri != 0 */
static seq_t coef12(idx_t m, int tri) {
    idx_t L = m + 1;
    idx_t cnt = 0, k;
    for (k = 1; ; k++) {
        idx_t e = tri ? k * (k + 1) : k * k;
        if (e > m) break;
        cnt++;
    }
    idx_t *pos = (idx_t *)malloc((cnt + 1) * sizeof(idx_t));
    for (k = 1; k <= cnt; k++) pos[k - 1] = tri ? k * (k + 1) : k * k;
    int w = tri ? 1 : 2;

    seq_t *g = alloc_zero(L);
    seq_t *h = alloc_zero(L);
    g[0] = 1;
    int step;
    for (step = 0; step < 12; step++) {
        idx_t i;
        for (i = 0; i < L; i++) {
            seq_t acc = g[i];
            idx_t j;
            for (j = 0; j < cnt && pos[j] <= i; j++)
                acc += g[i - pos[j]] * w;
            h[i] = acc;
        }
        seq_t *t = g; g = h; h = t;
    }
    seq_t r = g[m];
    release(g, L);
    release(h, L);
    free(pos);
    return r;
}

seq_t a(idx_t n) {
    seq_t r = 0;
    if (n % 2 == 0) r += coef12(n, 0);
    if (n >= 3) r += coef12(n - 3, 1) * 2048;
    return r;
}
