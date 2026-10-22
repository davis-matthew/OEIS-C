/*
 * A000306  Number of trees of diameter 8.
 * https://oeis.org/A000306
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = R4(n) - R3(n) - Sum_{i=1..n-1} R3(i)*(R3(n-i) - R2(n-i)),
 *            where Rh(n) = number of rooted trees with n nodes and height <=
 *            h, R0(x) = x, R_{h+1}(x) = x*exp(Sum_{k>=1} R_h(x^k)/k) (R2(n) =
 *            A000041(n-1)). Euler transform: R_{h+1}(t) = (1/(t-1)) *
 *            Sum_{k=1..t-1} c_h(k)*R_{h+1}(t-k), R_{h+1}(1)...
 * Range:     u8 n <= 13, u16 n <= 18, u32 n <= 33, u64 n <= 67, u128 n <= 139,
 *            big any n
 * Helpers:   a306_alloc0, a306_divsum, a306_fx_level, a306_grow,
 *            a306_inv_odd, a306_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/a306_alloc0.h"
#include "../utils/a306_divsum.h"
#include "../utils/a306_fx_level.h"
#include "../utils/a306_grow.h"
#include "../utils/a306_inv_odd.h"
#include "../utils/a306_mul.h"

static int a306_w = -1; /* bit width of seq_t, 0 for arbitrary precision */

static int a306_width(void)
{
    if (a306_w < 0) {
        seq_t x = 1;
        int w = 0;
        while (w < 200 && !(x == 0)) { x = (seq_t)(x * 2u); w++; }
        a306_w = (w < 200) ? w : 0;
    }
    return a306_w;
}

/* ---------- fixed-width path: division-free, exact modulo 2^w ---------- */

static seq_t *a306_fR2 = 0, *a306_fR3 = 0, *a306_fR4 = 0;
static idx_t a306_fcap = 0;
#define A306_FIXED_MAX 1000 /* a(n) exceeds 2^128 long before this */

static void a306_fx_extend(idx_t n, int w)
{
    idx_t N = n;
    if (N < 2 * a306_fcap) N = 2 * a306_fcap;
    if (N < 64) N = 64;
    if (N > A306_FIXED_MAX) N = A306_FIXED_MAX;
    seq_t *r0 = a306_alloc0(N + 1);
    r0[1] = 1;
    seq_t *r1 = a306_fx_level(r0, N, w);
    free(r0);
    seq_t *r2 = a306_fx_level(r1, N, w);
    free(r1);
    seq_t *r3 = a306_fx_level(r2, N, w);
    seq_t *r4 = a306_fx_level(r3, N, w);
    if (a306_fR2) { free(a306_fR2); free(a306_fR3); free(a306_fR4); }
    a306_fR2 = r2;
    a306_fR3 = r3;
    a306_fR4 = r4;
    a306_fcap = N;
}

/* ---------- arbitrary-precision path: incremental Euler transform ---------- */

static seq_t *a306_pt = 0, *a306_L2 = 0, *a306_L3 = 0, *a306_L4 = 0;
static seq_t *a306_c2 = 0, *a306_c3 = 0;
static idx_t a306_bcap = 0, a306_bn = 0;

static void a306_big_extend(idx_t N)
{
    if (N > a306_bcap) {
        idx_t nc = N;
        if (nc < 2 * a306_bcap) nc = 2 * a306_bcap;
        if (nc < 64) nc = 64;
        idx_t ol = a306_bcap ? a306_bcap + 1 : 0;
        a306_pt = a306_grow(a306_pt, ol, nc + 1);
        a306_L2 = a306_grow(a306_L2, ol, nc + 1);
        a306_L3 = a306_grow(a306_L3, ol, nc + 1);
        a306_L4 = a306_grow(a306_L4, ol, nc + 1);
        a306_c2 = a306_grow(a306_c2, ol, nc + 1);
        a306_c3 = a306_grow(a306_c3, ol, nc + 1);
        a306_bcap = nc;
    }
    for (idx_t t = a306_bn + 1; t <= N; t++) {
        idx_t m = t - 1;
        if (m == 0) a306_pt[0] = 1;
        else {
            seq_t pos = 0, neg = 0;
            for (idx_t k = 1;; k++) {
                idx_t g1 = k * (3 * k - 1) / 2;
                if (g1 > m) break;
                bool odd = (k & 1) != 0;
                if (odd) pos += a306_pt[m - g1]; else neg += a306_pt[m - g1];
                idx_t g2 = g1 + k;
                if (g2 <= m) {
                    if (odd) pos += a306_pt[m - g2]; else neg += a306_pt[m - g2];
                }
            }
            a306_pt[m] = pos - neg;
        }
        a306_L2[t] = a306_pt[m];
        if (t == 1) {
            a306_L3[1] = 1;
            a306_L4[1] = 1;
            continue;
        }
        a306_c2[m] = a306_divsum(a306_L2, m);
        a306_c3[m] = a306_divsum(a306_L3, m);
        seq_t s3 = 0, s4 = 0;
        for (idx_t i = 1; i <= m; i++) {
            s3 += a306_c2[i] * a306_L3[t - i];
            s4 += a306_c3[i] * a306_L4[t - i];
        }
        a306_L3[t] = s3 / (seq_t)m;
        a306_L4[t] = s4 / (seq_t)m;
    }
    if (N > a306_bn) a306_bn = N;
}

seq_t a(idx_t n)
{
    int w = a306_width();
    const seq_t *R2, *R3, *R4;
    if (w) {
        if (n > A306_FIXED_MAX) return 0; /* value cannot fit in seq_t */
        if (n > a306_fcap) a306_fx_extend(n, w);
        R2 = a306_fR2; R3 = a306_fR3; R4 = a306_fR4;
    } else {
        if (n > a306_bn) a306_big_extend(n);
        R2 = a306_L2; R3 = a306_L3; R4 = a306_L4;
    }
    seq_t s = 0;
    for (idx_t i = 1; i < n; i++)
        s = (seq_t)(s + a306_mul(R3[i], (seq_t)(R3[n - i] - R2[n - i])));
    seq_t top = (seq_t)(R4[n] - R3[n]);
    return (seq_t)(top - s);
}
