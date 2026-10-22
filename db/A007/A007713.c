/*
 * A007713  Number of 4-level rooted trees with n leaves.
 * https://oeis.org/A007713
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = EULER(EULER(EULER(1,1,1,...)))(n), i.e. the Euler
 *            transform applied three times to the all-1's sequence. For b =
 *            EULER(a): b(0) = 1, b(n) = (1/n) * Sum_{k=1..n} c(k)*b(n-k),
 *            where c(k) = Sum_{d|k} d*a(d). For fixed-width seq_t, b is found
 *            without that division: b = Product_{k>=1} 1/(1-x^k)...
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 25, u64 n <= 53, u128 n <= 113,
 *            big any n
 * Helpers:   gcd_seq, grow_zero, release, seq_is_fixed_width, zalloc_arr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd_seq.h"
#include "../utils/grow_zero.h"
#include "../utils/seq_is_fixed_width.h"
#include "../utils/zalloc_arr.h"
#include "../utils/release.h"

static seq_t *E_[4];
static seq_t *C_[3];
static idx_t have_ = 0, cap_ = 0;

static seq_t a_big(idx_t n) {
    if (n + 1 > cap_) {
        idx_t nc = cap_ ? cap_ : 16;
        while (nc < n + 1) nc *= 2;
        for (int s = 0; s < 4; s++) E_[s] = grow_zero(E_[s], cap_, nc);
        for (int s = 0; s < 3; s++) C_[s] = grow_zero(C_[s], cap_, nc);
        cap_ = nc;
    }
    for (idx_t i = have_; i <= n; i++) {
        E_[0][i] = 1;
        for (int s = 0; s < 3; s++) {
            if (i == 0) { E_[s + 1][0] = 1; continue; }
            seq_t c = 0;
            for (idx_t d = 1; d * d <= i; d++) {
                if (i % d == 0) {
                    idx_t e = i / d;
                    c += (seq_t)d * E_[s][d];
                    if (e != d) c += (seq_t)e * E_[s][e];
                }
            }
            C_[s][i] = c;
            seq_t sum = 0;
            for (idx_t k = 1; k <= i; k++) sum += C_[s][k] * E_[s + 1][i - k];
            E_[s + 1][i] = sum / (seq_t)i;
        }
    }
    if (n + 1 > have_) have_ = n + 1;
    return E_[3][n];
}

static seq_t a_fixed(idx_t n) {
    idx_t N = n;
    idx_t len = N + 1;
    seq_t *cur = zalloc_arr(len);
    seq_t *g = zalloc_arr(len);
    for (idx_t i = 0; i < len; i++) cur[i] = 1;
    for (int s = 0; s < 3; s++) {
        seq_t *P = zalloc_arr(len);
        P[0] = 1;
        for (idx_t k = 1; k <= N; k++) {
            seq_t m = cur[k];
            idx_t J = N / k;
            g[0] = 1;
            for (idx_t j = 1; j <= J; j++) {
                seq_t t = gcd_seq(g[j - 1], (seq_t)j);
                seq_t gp = (seq_t)(g[j - 1] / t);
                seq_t jj = (seq_t)((seq_t)j / t);
                seq_t num = (seq_t)(m + (seq_t)(j - 1));
                seq_t q = (seq_t)(num / jj);
                g[j] = (seq_t)(gp * q);
            }
            for (idx_t i = N; i >= k; i--) {
                seq_t acc = P[i];
                for (idx_t j = 1; j * k <= i; j++)
                    acc = (seq_t)(acc + (seq_t)(g[j] * P[i - j * k]));
                P[i] = acc;
            }
        }
        release(cur, len);
        cur = P;
    }
    seq_t r = cur[N];
    release(cur, len);
    release(g, len);
    return r;
}

seq_t a(idx_t n) {
    if (seq_is_fixed_width()) {
        /* a(n) > 2^128 for all n > 120, so such terms cannot fit any fixed width */
        if (n > 300) return 0;
        return a_fixed(n);
    }
    return a_big(n);
}
