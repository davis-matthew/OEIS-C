/*
 * A001853  Total height of trees with n nodes.
 * https://oeis.org/A001853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{h=0..n-2} (A000081(n) - R_h(n)) = Sum_{k=1..n-1}
 *            A034781(n,k)*k, where R_h(n) = number of rooted trees with n
 *            nodes and height <= h: R_0(x) = x, R_h(x) = x*exp(Sum_{k>=1}
 *            R_{h-1}(x^k)/k), computed by the Euler transform recurrence
 *            m*R_h(m+1) = Sum_{k=1..m} (Sum_{d|k} d*R_{h-1}(d)) * R...
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 23, u64 n <= 44, u128 n <= 86,
 *            big any n
 * Helpers:   a1853_mod_run, a1853_powm, a1853_width, a1853_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/a1853_mod_run.h"
#include "../utils/a1853_powm.h"
#include "../utils/a1853_width.h"
#include "../utils/a1853_zero.h"

static seq_t *a1853_tot = NULL;
static idx_t a1853_N = 0;

static void a1853_big_compute(idx_t N) {
    seq_t *T = a1853_zero(N + 2), *P = a1853_zero(N + 2), *R = a1853_zero(N + 2);
    seq_t *c = a1853_zero(N + 2), *tot = a1853_zero(N + 2);
    idx_t m, k, d, h, j;
    T[1] = 1;
    for (m = 1; m < N; m++) {
        for (k = m; k <= N; k += m) c[k] += T[m] * (seq_t)m;
        seq_t s = 0;
        for (k = 1; k <= m; k++) s += c[k] * T[m + 1 - k];
        T[m + 1] = s / (seq_t)m;
    }
    /* level h = 0: R_0(m) = [m == 1] */
    for (m = 0; m <= N; m++) P[m] = 0;
    P[1] = 1;
    for (m = 2; m <= N; m++) tot[m] = T[m];
    for (h = 1; h + 2 <= N; h++) {
        for (k = 0; k <= N; k++) c[k] = 0;
        for (d = 1; d <= N; d++) {
            seq_t v = P[d] * (seq_t)d;
            for (k = d; k <= N; k += d) c[k] += v;
        }
        for (j = 1; j <= h + 1; j++) R[j] = T[j];
        for (m = h + 1; m < N; m++) {
            seq_t s = 0;
            for (k = 1; k <= m; k++) s += c[k] * R[m + 1 - k];
            R[m + 1] = s / (seq_t)m;
        }
        for (j = h + 2; j <= N; j++) tot[j] += T[j] - R[j];
        seq_t *tmp = P; P = R; R = tmp;
    }
    free(T); free(P); free(R); free(c);
    if (a1853_tot) free(a1853_tot);
    a1853_tot = tot;
    a1853_N = N;
}

static const uint64_t a1853_pr[5] = {998244353ULL, 1000000007ULL, 1000000009ULL,
                                     1004535809ULL, 469762049ULL};

seq_t a(idx_t n) {
    int w = a1853_width();
    if (n <= 1) return 0;
    if (w >= 400) {
        if (n > a1853_N) a1853_big_compute(n + n / 4 + 8);
        return a1853_tot[n];
    }
    /* a(n) >= A000081(n) >= 2^(n-2): cannot fit in w bits */
    if (n >= (idx_t)w + 2) return 0;
    uint64_t r[5], v[5];
    int i, j;
    for (i = 0; i < 5; i++) r[i] = a1853_mod_run(n, a1853_pr[i]);
    for (i = 0; i < 5; i++) {
        uint64_t p = a1853_pr[i];
        uint64_t t = r[i] % p;
        for (j = 0; j < i; j++) {
            t = (t + p - v[j] % p) % p;
            t = t * a1853_powm(a1853_pr[j] % p, p - 2, p) % p;
        }
        v[i] = t;
    }
    if (w <= 64) {
        uint64_t y = 0;
        for (i = 4; i >= 0; i--) y = y * a1853_pr[i] + v[i];
        return (seq_t)y;
    } else {
        seq_t x = 0;
        for (i = 4; i >= 0; i--) x = x * (seq_t)a1853_pr[i] + (seq_t)v[i];
        return x;
    }
}
