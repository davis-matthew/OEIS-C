/*
 * A000429  Number of n-node rooted trees of height 8.
 * https://oeis.org/A000429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A034825(n) - A034824(n), where t_h(n) = number of rooted
 *            trees with height <= h: t_0(n) = [n = 1]; t_h(1) = 1, t_h(m+1) =
 *            f_h(m), f_h(0) = 1, m*f_h(m) = Sum_{k=1..m} c_h(k)*f_h(m-k),
 *            c_h(k) = Sum_{d|k} d*t_{h-1}(d) (Euler transform); a(n) = t_8(n)
 *            - t_7(n)
 * Range:     u8 n <= 12, u16 n <= 16, u32 n <= 27, u64 n <= 50, u128 n <= 97,
 *            big any n
 * Helpers:   g429_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/g429_zero.h"

/* ---------- fixed-width builds: exact small bignum, n <= Q429_N ---------- */
#define Q429_L 16
#define Q429_N 200

static seq_t *q429_tab = NULL;

static void q429_addmul(uint32_t *acc, const uint32_t *x, const uint32_t *y) {
    for (int i = 0; i < Q429_L; i++) {
        if (!x[i]) continue;
        uint64_t carry = 0;
        for (int j = 0; i + j < Q429_L; j++) {
            uint64_t cur = (uint64_t)acc[i + j] + (uint64_t)x[i] * y[j] + carry;
            acc[i + j] = (uint32_t)cur;
            carry = cur >> 32;
        }
    }
}

static void q429_addmulsmall(uint32_t *acc, const uint32_t *x, uint32_t m) {
    uint64_t carry = 0;
    for (int i = 0; i < Q429_L; i++) {
        uint64_t cur = (uint64_t)acc[i] + (uint64_t)x[i] * m + carry;
        acc[i] = (uint32_t)cur;
        carry = cur >> 32;
    }
}

static void q429_divsmall(uint32_t *x, uint32_t m) {
    uint64_t rem = 0;
    for (int i = Q429_L - 1; i >= 0; i--) {
        uint64_t cur = (rem << 32) | x[i];
        x[i] = (uint32_t)(cur / m);
        rem = cur % m;
    }
}

static bool q429_nonzero(const uint32_t *x) {
    for (int i = 0; i < Q429_L; i++) if (x[i]) return true;
    return false;
}

static void q429_build(void) {
    const int N = Q429_N, L = Q429_L;
    uint32_t *tp = (uint32_t *)calloc((size_t)(N + 1) * L, sizeof(uint32_t));
    uint32_t *tn = NULL, *t7 = NULL;
    tp[1 * L] = 1;
    for (int h = 1; h <= 8; h++) {
        uint32_t *c = (uint32_t *)calloc((size_t)(N + 1) * L, sizeof(uint32_t));
        for (int d = 1; d <= N; d++) {
            if (!q429_nonzero(tp + d * L)) continue;
            for (int k = d; k <= N; k += d)
                q429_addmulsmall(c + k * L, tp + d * L, (uint32_t)d);
        }
        uint32_t *f = (uint32_t *)calloc((size_t)(N + 1) * L, sizeof(uint32_t));
        f[0] = 1;
        for (int m = 1; m < N; m++) {
            uint32_t *s = f + m * L;
            for (int k = 1; k <= m; k++)
                q429_addmul(s, c + k * L, f + (m - k) * L);
            q429_divsmall(s, (uint32_t)m);
        }
        tn = (uint32_t *)calloc((size_t)(N + 1) * L, sizeof(uint32_t));
        for (int m = 0; m < N; m++)
            memcpy(tn + (m + 1) * L, f + m * L, (size_t)L * sizeof(uint32_t));
        free(c);
        free(f);
        if (h == 8) t7 = tp;
        else { free(tp); tp = tn; }
    }
    q429_tab = (seq_t *)calloc((size_t)(N + 1), sizeof(seq_t));
    for (int i = 0; i <= N; i++) {
        uint32_t diff[Q429_L];
        uint64_t borrow = 0;
        for (int j = 0; j < L; j++) {
            uint64_t a1 = tn[i * L + j], b1 = (uint64_t)t7[i * L + j] + borrow;
            diff[j] = (uint32_t)(a1 - b1);
            borrow = (a1 < b1) ? 1 : 0;
        }
        seq_t r = 0;
        for (int j = L - 1; j >= 0; j--) {
            r *= 256; r *= 256; r *= 256; r *= 256;
            r += (seq_t)diff[j];
        }
        q429_tab[i] = r;
    }
    free(tn);
    free(t7);
}

/* ---------- arbitrary-precision build: compute directly in seq_t ---------- */
static seq_t *g429_res = NULL;
static idx_t g429_N = 0;

static void g429_build(idx_t N) {
    seq_t *tp = g429_zero(N + 1), *tn = NULL, *t7 = NULL;
    tp[1] = 1;
    for (int h = 1; h <= 8; h++) {
        seq_t *c = g429_zero(N + 1);
        for (idx_t d = 1; d <= N; d++) {
            if (tp[d] == 0) continue;
            seq_t v = tp[d] * (seq_t)d;
            for (idx_t k = d; k <= N; k += d) c[k] += v;
        }
        seq_t *f = g429_zero(N + 1);
        f[0] = 1;
        for (idx_t m = 1; m < N; m++) {
            seq_t s = 0;
            for (idx_t k = 1; k <= m; k++) s += c[k] * f[m - k];
            f[m] = s / (seq_t)m;
        }
        tn = g429_zero(N + 1);
        for (idx_t m = 0; m < N; m++) tn[m + 1] = f[m];
        free(c);
        free(f);
        if (h == 8) t7 = tp;
        else { free(tp); tp = tn; }
    }
    if (g429_res) free(g429_res);
    g429_res = g429_zero(N + 1);
    for (idx_t i = 0; i <= N; i++) g429_res[i] = tn[i] - t7[i];
    free(tn);
    free(t7);
    g429_N = N;
}

seq_t a(idx_t n) {
    seq_t z = 0;
    z -= 1;
    bool fixed = (z > 0);
    if (fixed) {
        if (n > (idx_t)Q429_N) return 0; /* far beyond any fixed width */
        if (!q429_tab) q429_build();
        return q429_tab[n];
    }
    if (n > g429_N) {
        idx_t N = n;
        if (N < 2 * g429_N) N = 2 * g429_N;
        if (N < 64) N = 64;
        g429_build(N);
    }
    return g429_res[n];
}
