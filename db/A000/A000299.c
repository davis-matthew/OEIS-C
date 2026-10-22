/*
 * A000299  Number of n-node rooted trees of height 4.
 * https://oeis.org/A000299
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001384(n) - A001383(n), where t_h(n) is the number of
 *            n-node rooted trees of height at most h: t_1(n) = 1, and
 *            t_h(n+1) = c(n) with c(0) = 1, n*c(n) = Sum_{k=1..n}
 *            s(k)*c(n-k), s(k) = Sum_{d|k} d*t_{h-1}(d) (Euler transform).
 *            a(n) = t_4(n) - t_3(n)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 33, u64 n <= 67, u128 n <= 139,
 *            big any n
 * Helpers:   my_fixed_width, zalloc_s
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/zalloc_s.h"
#include "../utils/my_fixed_width.h"

static int fw_mode = -1;
static idx_t cap = 0;
static seq_t *T3 = 0, *T4 = 0;

/* out[m] = number of m-node rooted trees whose subtrees are counted by t */
static void euler(const seq_t *t, seq_t *out, idx_t N, seq_t *s, seq_t *c)
{
    idx_t d, k, m;
    for (k = 0; k <= N; k++) s[k] = 0;
    for (d = 1; d <= N; d++)
        for (k = d; k <= N; k += d)
            s[k] += t[d] * (seq_t)d;
    c[0] = 1;
    for (m = 1; m < N; m++) {
        seq_t mm = (seq_t)m;
        if (fw_mode) {
            /* accumulate sum/m as quotient + remainder to avoid overflow */
            seq_t Q = 0, R = 0;
            for (k = 1; k <= m; k++) {
                seq_t sk = s[k], cv = c[m - k];
                seq_t qs = sk / mm, rs = sk % mm;
                seq_t qc = cv / mm, rc = cv % mm;
                Q += qs * cv + rs * qc;
                R += rs * rc;
                if (R >= mm) {
                    Q += R / mm;
                    R %= mm;
                }
            }
            Q += R / mm;
            c[m] = Q;
        } else {
            seq_t S = 0;
            for (k = 1; k <= m; k++)
                S += s[k] * c[m - k];
            c[m] = S / mm;
        }
    }
    out[0] = 0;
    for (m = 1; m <= N; m++) out[m] = c[m - 1];
}

static void build(idx_t N)
{
    idx_t i;
    seq_t *t1 = zalloc_s(N + 1);
    seq_t *t2 = zalloc_s(N + 1);
    seq_t *t3 = zalloc_s(N + 1);
    seq_t *t4 = zalloc_s(N + 1);
    seq_t *s = zalloc_s(N + 1);
    seq_t *c = zalloc_s(N + 1);
    t1[0] = 0;
    for (i = 1; i <= N; i++) t1[i] = 1;
    euler(t1, t2, N, s, c);
    euler(t2, t3, N, s, c);
    euler(t3, t4, N, s, c);
    T3 = t3;
    T4 = t4;
    cap = N;
}

seq_t a(idx_t n)
{
    if (fw_mode < 0) fw_mode = my_fixed_width();
    if (n > cap) {
        idx_t N = cap * 2;
        if (N < n) N = n;
        if (N < 64) N = 64;
        build(N);
    }
    return T4[n] - T3[n];
}
