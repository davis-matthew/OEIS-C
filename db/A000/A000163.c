/*
 * A000163  Series-parallel numbers.
 * https://oeis.org/A000163
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   G.f.: 2*(1+S)/(1-S)^3, where S = g.f. for A000084 (S = x + 2x^2
 *            + 4x^3 + ...). A000084: S(1) = 1, S(n) = 2*b(n) for n >= 2,
 *            where b = A000669 satisfies 2*B(x) = x - 1 + Prod_{k>=1}
 *            1/(1-x^k)^b(k). Here a(n) = [x^(n-2)] 2*(1+S)/(1-S)^3
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 35, u128 n <= 69,
 *            big any n
 * Helpers:   detect_fixed, gcd_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/detect_fixed.h"
#include "../utils/gcd_idx.h"

#define MAXN 20010
#define FMAX 160

static seq_t Bv[MAXN], Cv[MAXN], Dv[MAXN], Sv[MAXN], Rv[MAXN], R2v[MAXN], Tv[MAXN];
static seq_t Pf[FMAX + 2], Cb[FMAX + 2];
static int mode = -1;          /* 1 = fixed width, 0 = arbitrary precision */
static idx_t sdone = 0;        /* Sv[1..sdone] valid */
static idx_t tdone = 0;        /* Rv, R2v, Tv [0..tdone-1] valid */

/* Division-free (except exact small divisions) computation of S[1..M] for fixed-width builds. */
static void fixed_fill(idx_t M)
{
    idx_t i, m, j, k;
    for (i = 0; i <= M; i++) Pf[i] = 0;
    Pf[0] = 1;
    for (m = 1; m <= M; m++) {
        idx_t jmax;
        if (m == 1) Bv[1] = 1; else Bv[1 + 0] = Bv[1], Bv[m] = Pf[m];
        jmax = M / m;
        Cb[0] = 1;
        for (j = 1; j <= jmax; j++) {
            /* C(B+j-1, j) = C(B+j-2, j-1) * (B+j-1) / j, without overflow */
            seq_t t = Bv[m];
            idx_t r, g, jp;
            t += (seq_t)(j - 1);
            r = (idx_t)(t % j);
            g = gcd_idx(r, j);
            jp = j / g;
            Cb[j] = (Cb[j - 1] / jp) * (t / g);
        }
        for (k = M; k >= m; k--) {
            seq_t acc = Pf[k];
            for (j = 1; j * m <= k; j++) acc += Cb[j] * Pf[k - j * m];
            Pf[k] = acc;
            if (k == m) break;
        }
    }
    Sv[1] = 1;
    for (k = 2; k <= M; k++) Sv[k] = Bv[k] * 2;
    sdone = M;
}

/* Euler-transform recurrence with exact division, for arbitrary-precision builds. */
static void big_fill(idx_t M)
{
    idx_t n, k, j;
    if (sdone == 0) {
        Bv[1] = 1; Cv[0] = 1; Cv[1] = 1; Dv[1] = 1; Sv[1] = 1;
        sdone = 1;
    }
    for (n = sdone + 1; n <= M; n++) {
        seq_t sum = 0, dp = 0;
        for (k = 1; k < n; k++) sum += Dv[k] * Cv[n - k];
        for (j = 1; j * j <= n; j++) {
            if (n % j == 0) {
                idx_t o = n / j;
                if (j < n) dp += (seq_t)j * Bv[j];
                if (o != j && o < n) dp += (seq_t)o * Bv[o];
            }
        }
        Bv[n] = (sum + dp) / n;
        Cv[n] = Bv[n] * 2;
        Dv[n] = dp + (seq_t)n * Bv[n];
        Sv[n] = Cv[n];
    }
    sdone = M;
}

seq_t a(idx_t n)
{
    idx_t m, k, i;
    seq_t ans;
    if (n < 2) return 0;
    m = n - 2;
    if (mode < 0) mode = detect_fixed();
    if (mode == 1) {
        if (m > FMAX) return 0;
        if (sdone == 0) fixed_fill(FMAX);
    } else {
        if (m >= MAXN - 2) return 0;
        if (m > sdone) big_fill(m);
    }
    if (tdone == 0) {
        Rv[0] = 1; R2v[0] = 1; Tv[0] = 1;
        tdone = 1;
    }
    for (i = tdone; i <= m; i++) {
        seq_t r = 0, r2 = 0, t = 0;
        for (k = 1; k <= i; k++) r += Sv[k] * Rv[i - k];
        Rv[i] = r;
        for (k = 0; k <= i; k++) r2 += Rv[k] * Rv[i - k];
        R2v[i] = r2;
        for (k = 0; k <= i; k++) t += R2v[k] * Rv[i - k];
        Tv[i] = t;
    }
    if (m + 1 > tdone) tdone = m + 1;
    ans = Tv[m];
    for (k = 1; k <= m; k++) ans += Sv[k] * Tv[m - k];
    return ans * 2;
}
