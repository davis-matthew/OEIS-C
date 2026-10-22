/*
 * A000465  Number of bipartite partitions of n white objects and 4 black
 *          ones.
 * https://oeis.org/A000465
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{t=0..n} r(t)*A000041(n-t), where r has g.f. R(x) =
 *            1/(1-x) + 1/(1-x)^2 + 1/((1-x)(1-x^2)) + 1/((1-x)^2(1-x^2)) +
 *            1/((1-x)(1-x^2)(1-x^3)(1-x^4)), i.e. the coefficient of y^4 in
 *            Prod_{i>=0,j>=1} 1/(1-x^i*y^j), one term per partition of 4. The
 *            blocks that contain no black objects contrib...
 * Range:     u8 n <= 5, u16 n <= 20, u32 n <= 75, u64 n <= 291,
 *            u128 n <= 1156, big any n
 * Helpers:   alloc_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/alloc_zero.h"

static seq_t *Pp = 0;
static seq_t *Rr = 0;
static seq_t *Tt = 0;
static idx_t cap = 0;

static void build(idx_t N) {
    static const int comps[5][4] = {
        {1, 0, 0, 0}, {1, 1, 0, 0}, {1, 2, 0, 0}, {1, 1, 2, 0}, {1, 2, 3, 4}
    };
    idx_t i, m, k;
    int c, d;
    Pp = alloc_zero(N + 1);
    Rr = alloc_zero(N + 1);
    Tt = alloc_zero(N + 1);
    cap = N;

    /* partition numbers */
    Pp[0] = 1;
    for (m = 1; m <= N; m++) {
        seq_t pos = 0, neg = 0;
        for (k = 1;; k++) {
            idx_t g1 = k * (3 * k - 1) / 2;
            idx_t g2 = g1 + k;
            if (g1 > m) break;
            if (k & 1) {
                pos += Pp[m - g1];
                if (g2 <= m) pos += Pp[m - g2];
            } else {
                neg += Pp[m - g1];
                if (g2 <= m) neg += Pp[m - g2];
            }
        }
        Pp[m] = pos - neg;
    }

    /* r(t) */
    for (c = 0; c < 5; c++) {
        for (i = 0; i <= N; i++) Tt[i] = 0;
        Tt[0] = 1;
        for (d = 0; d < 4; d++) {
            idx_t s = (idx_t)comps[c][d];
            if (s == 0) break;
            for (i = s; i <= N; i++) Tt[i] += Tt[i - s];
        }
        for (i = 0; i <= N; i++) Rr[i] += Tt[i];
    }
}

seq_t a(idx_t n) {
    idx_t t;
    seq_t s = 0;
    if (Pp == 0 || n > cap) {
        idx_t N = n < 64 ? 64 : n;
        if (Pp != 0 && N < 2 * cap) N = 2 * cap;
        build(N);
    }
    for (t = 0; t <= n; t++) s += Rr[t] * Pp[n - t];
    return s;
}
