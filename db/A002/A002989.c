/*
 * A002989  Number of n-node trees with a forbidden limb of length 3.
 * https://oeis.org/A002989
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [n=0] + [1<=n<=3] + C0(n) + H(n-1) - H(n) - S2F(n-1) + [n
 *            even]*C(n/2), where (planted trees, x = one node): F(x) = x +
 *            x^2 + C(x) (limbs of length 0 or 1, or closed subtrees) G(x) =
 *            MSET(F) = Euler transform of F C0(x) = x*(G(x) - 1 - F(x)) (the
 *            top node branches) C(x) = C0(x)/(1-x) (clos...
 * Range:     u8 n <= 11, u16 n <= 18, u32 n <= 30, u64 n <= 52, u128 n <= 96,
 *            big any n
 * Helpers:   A_grow, a2989_mc2, a2989_mcj, a2989_sgcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/A_grow.h"
#include "../utils/a2989_mc2.h"
#include "../utils/a2989_mcj.h"
#include "../utils/a2989_sgcd.h"

static seq_t *s_f = NULL, *s_C0 = NULL, *s_C = NULL, *s_G = NULL, *s_cc = NULL;
static seq_t *s_H = NULL, *s_MS = NULL, *s_A = NULL;
static idx_t s_len = 0;   /* allocated length */
static idx_t s_N = 0;     /* computed up to */
static int s_init = 0, s_fixed = 0, s_W = 0;

static void a2989_compute(idx_t N)
{
    idx_t m, k, w, j, i;
    idx_t len = N + 1;
    if (len > s_len) {
        s_f  = A_grow(s_f,  s_len, len);
        s_C0 = A_grow(s_C0, s_len, len);
        s_C  = A_grow(s_C,  s_len, len);
        s_G  = A_grow(s_G,  s_len, len);
        s_cc = A_grow(s_cc, s_len, len);
        s_H  = A_grow(s_H,  s_len, len);
        s_MS = A_grow(s_MS, s_len, len);
        s_A  = A_grow(s_A,  s_len, len);
        s_len = len;
    }
    for (i = 0; i <= N; i++) {
        s_f[i] = 0; s_C0[i] = 0; s_C[i] = 0; s_G[i] = 0; s_cc[i] = 0;
        s_H[i] = 0; s_MS[i] = 0; s_A[i] = 0;
    }
    s_G[0] = 1;
    for (m = 1; m <= N; m++) {
        s_C0[m] = s_G[m - 1] - s_f[m - 1];
        if (m == 1) s_C0[m] -= 1;
        s_C[m] = s_C0[m] + s_C[m - 1];
        s_f[m] = s_C[m];
        if (m <= 2) s_f[m] += 1;
        if (!s_fixed) {
            seq_t s = 0;
            seq_t add = s_f[m] * (seq_t)m;
            for (k = m; k <= N; k += m) s_cc[k] += add;
            for (k = 1; k <= m; k++) s += s_cc[k] * s_G[m - k];
            s_G[m] = s / (seq_t)m;
        } else {
            seq_t mcv[300];
            idx_t jmax = N / m;
            for (j = 1; j <= jmax; j++) mcv[j] = a2989_mcj(s_f[m], j);
            for (w = N; w >= m; w--) {
                seq_t s = 0;
                for (j = 1; j * m <= w; j++) s += mcv[j] * s_G[w - j * m];
                s_G[w] += s;
            }
        }
    }
    for (m = 1; m <= N; m++) {
        seq_t h = 0, ms = 0;
        for (i = 1; 2 * i < m; i++) {
            h += s_C[i] * s_C[m - i];
            ms += s_f[i] * s_f[m - i];
        }
        if (m % 2 == 0) {
            h += a2989_mc2(s_C[m / 2]);
            ms += a2989_mc2(s_f[m / 2]);
        }
        s_H[m] = h;
        s_MS[m] = ms;
    }
    s_A[0] = 1;
    for (m = 1; m <= N; m++) {
        seq_t v = s_C0[m];
        if (m <= 3) v += 1;
        v += s_H[m - 1];
        v -= s_H[m];
        v -= s_MS[m - 1];
        if (m % 2 == 0) v += s_C[m / 2];
        s_A[m] = v;
    }
    s_N = N;
}

seq_t a(idx_t n)
{
    if (!s_init) {
        seq_t x = 1;
        int bits = 0;
        while (x != 0 && bits < 300) { x += x; bits++; }
        s_fixed = (x == 0);
        s_W = bits;
        s_init = 1;
        if (s_fixed) a2989_compute((idx_t)(2 * s_W));
    }
    if (n == 0) return 1;
    if (s_fixed) {
        if (n > (idx_t)(2 * s_W)) return 0; /* value cannot fit anyway */
        return s_A[n];
    }
    if (n > s_N || s_N == 0) {
        idx_t N = 2 * s_N;
        if (N < n) N = n;
        if (N < 64) N = 64;
        a2989_compute(N);
    }
    return s_A[n];
}
