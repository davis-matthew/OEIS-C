/*
 * A005197  a(n) = Sum_t t*F(n,t), where F(n,t) (see A033185) is the number of
 *          rooted forests with n (unlabeled) nodes and exactly t rooted
 *          trees.
 * https://oeis.org/A005197
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n-1} A000081(k+1) * b(n-k), where b(m) =
 *            Sum_{d|m} A000081(d); i.e. G.f. = (Sum_{n>=0} A000081(n+1) x^n)
 *            * Sum_{k>=1} A000081(k) x^k/(1-x^k). A000081: r(1) = 1, r(m+1) =
 *            (1/m) Sum_{k=1..m} (Sum_{d|k} d*r(d)) * r(m+1-k). Fixed-width
 *            builds use the division-free Euler product Prod_...
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 23, u64 n <= 45, u128 n <= 87,
 *            big any n
 * Helpers:   alloc_zero, grow_zero, release, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/grow_zero.h"
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/seq_is_fixed_width.h"

static seq_t *R_ = NULL, *S_ = NULL, *B_ = NULL;
static idx_t have_ = 0, cap_ = 0;

static void ensure(idx_t M) {
    if (M <= have_) return;
    if (M + 2 > cap_) {
        idx_t nc = M + 2;
        if (nc < 2 * cap_) nc = 2 * cap_;
        R_ = grow_zero(R_, cap_, nc);
        S_ = grow_zero(S_, cap_, nc);
        B_ = grow_zero(B_, cap_, nc);
        cap_ = nc;
    }
    if (have_ == 0) R_[1] = 1;
    for (idx_t m = have_ + 1; m <= M; m++) {
        seq_t s = 0, b = 0;
        for (idx_t d = 1; d * d <= m; d++) {
            if (m % d == 0) {
                idx_t e = m / d;
                s += (seq_t)d * R_[d];
                b += R_[d];
                if (e != d) {
                    s += (seq_t)e * R_[e];
                    b += R_[e];
                }
            }
        }
        S_[m] = s;
        B_[m] = b;
        seq_t sum = 0;
        for (idx_t k = 1; k <= m; k++) sum += S_[k] * R_[m + 1 - k];
        R_[m + 1] = sum / (seq_t)m;
    }
    have_ = M;
}

/* C(Rv + j - 1, j) without division overflow: cancel j! against the numerator terms. */
static seq_t mbinom(seq_t Rv, idx_t j) {
    seq_t *t = alloc_zero(j);
    for (idx_t i = 0; i < j; i++) t[i] = Rv + (seq_t)i;
    for (idx_t d = 2; d <= j; d++) {
        idx_t x = d;
        for (idx_t p = 2; x > 1; p++) {
            if (p * p > x) p = x;
            while (x % p == 0) {
                x /= p;
                for (idx_t i = 0; i < j; i++) {
                    if (t[i] % (seq_t)p == 0) {
                        t[i] /= (seq_t)p;
                        break;
                    }
                }
            }
        }
    }
    seq_t res = 1;
    for (idx_t i = 0; i < j; i++) res *= t[i];
    release(t, j);
    return res;
}

static seq_t a_fixed(idx_t n) {
    seq_t *g = alloc_zero(n + 2);
    seq_t *r = alloc_zero(n + 2);
    seq_t *c = alloc_zero(n + 2);
    g[0] = 1;
    for (idx_t k = 1; k <= n; k++) {
        r[k] = g[k - 1];
        idx_t J = n / k;
        for (idx_t j = 1; j <= J; j++) c[j] = mbinom(r[k], j);
        for (idx_t m = n; m >= k; m--) {
            seq_t acc = 0;
            for (idx_t j = 1; j * k <= m; j++) acc += g[m - j * k] * c[j];
            g[m] += acc;
        }
    }
    seq_t res = 0;
    for (idx_t i = 1; i <= n; i++) {
        seq_t bi = 0;
        for (idx_t d = 1; d <= i; d++)
            if (i % d == 0) bi += r[d];
        res += g[n - i] * bi;
    }
    release(g, n + 2);
    release(r, n + 2);
    release(c, n + 2);
    return res;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (seq_is_fixed_width() && n <= 150) return a_fixed(n);
    ensure(n);
    seq_t res = 0;
    for (idx_t k = 0; k < n; k++) res += R_[k + 1] * B_[n - k];
    return res;
}
