/*
 * A003214  Number of binary forests with n nodes.
 * https://oeis.org/A003214
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (1/n) * Sum_{k=1..n} c(k)*a(n-k), a(0) = 1, where c(k) =
 *            Sum_{d|k} d*A001190(d) (Euler transform of A001190); A001190:
 *            W(1) = 1, W(m) = Sum_{i<m/2} W(i)W(m-i) + [m even]
 *            W(m/2)(W(m/2)+1)/2
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 29, u64 n <= 55, u128 n <= 105,
 *            big any n
 * Helpers:   a3214_W, a3214_bits, a3214_halfpair, a3214_inv, a3214_isfixed
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/a3214_W.h"
#include "../utils/a3214_bits.h"
#include "../utils/a3214_halfpair.h"
#include "../utils/a3214_inv.h"
#include "../utils/a3214_isfixed.h"

#define A3214_MAXF 300

static seq_t a3214_fixed_calc(idx_t n) {
    int w = a3214_bits();
    idx_t cap = (idx_t)(2 * w + 16);
    if (n > cap) return 0; /* value cannot fit in seq_t */
    static seq_t W[A3214_MAXF], f[A3214_MAXF], C[A3214_MAXF];
    idx_t N = n;
    for (idx_t m = 0; m <= N; m++) { a3214_W(W, m); f[m] = 0; }
    f[0] = 1;
    for (idx_t k = 1; k <= N; k++) {
        seq_t c = W[k];
        idx_t J = N / k;
        seq_t O = 1;
        long long e = 0;
        C[0] = 1;
        for (idx_t t = 1; t <= J; t++) {
            seq_t num = c + (seq_t)(t - 1);
            long long vn = 0;
            if (num == 0) { vn = w; num = 1; }
            else while (num % 2 == 0) { num /= 2; vn++; }
            idx_t tt = t;
            long long vt = 0;
            while (tt % 2 == 0) { tt /= 2; vt++; }
            O = O * num * a3214_inv((seq_t)tt);
            e += vn - vt;
            if (e < 0 || e >= w) C[t] = 0;
            else {
                seq_t v = O;
                for (long long s = 0; s < e; s++) v *= 2;
                C[t] = v;
            }
        }
        for (idx_t m = N; m >= k; m--) {
            seq_t s = 0;
            for (idx_t j = 1; j * k <= m; j++) s += f[m - j * k] * C[j];
            f[m] += s;
        }
    }
    return f[N];
}

#define A3214_MAXB 16384

static seq_t a3214_SW[A3214_MAXB], a3214_SC[A3214_MAXB], a3214_SA[A3214_MAXB];
static idx_t a3214_len = 0;

static seq_t a3214_big_calc(idx_t n) {
    if (n >= A3214_MAXB) return 0;
    if (n < a3214_len) return a3214_SA[n];
    for (idx_t m = a3214_len; m <= n; m++) {
        a3214_W(a3214_SW, m);
        if (m == 0) {
            a3214_SC[0] = 0;
            a3214_SA[0] = 1;
            continue;
        }
        seq_t cs = 0;
        for (idx_t d = 1; d * d <= m; d++) {
            if (m % d == 0) {
                cs += (seq_t)d * a3214_SW[d];
                idx_t e = m / d;
                if (e != d) cs += (seq_t)e * a3214_SW[e];
            }
        }
        a3214_SC[m] = cs;
        seq_t s = 0;
        for (idx_t k = 1; k <= m; k++) s += a3214_SC[k] * a3214_SA[m - k];
        a3214_SA[m] = s / (seq_t)m;
    }
    a3214_len = n + 1;
    return a3214_SA[n];
}

seq_t a(idx_t n) {
    if (a3214_isfixed()) return a3214_fixed_calc(n);
    return a3214_big_calc(n);
}
