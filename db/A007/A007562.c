/*
 * A007562  Number of planted trees where non-root, non-leaf nodes an even
 *          distance from root are of degree 2.
 * https://oeis.org/A007562
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, eigen
 * Formula:   G.f. A(x) = x + x^2 / Product_{k>0} (1 - x^k)^a(k), i.e. a(1) =
 *            1 and a(n) = b(n-2) for n >= 2, where b is the Euler transform
 *            of a: b(0) = 1, b(m) = (1/m) * Sum_{k=1..m} c(k)*b(m-k), c(k) =
 *            Sum_{d|k} d*a(d). For fixed-width seq_t the product is expanded
 *            factor by factor without division, using b...
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 34, u64 n <= 62, u128 n <= 118,
 *            big any n
 * Helpers:   alloc_zero, gcd_seq, grow_zero, release, seq_is_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd_seq.h"
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"
#include "../utils/release.h"
#include "../utils/seq_is_big.h"

static seq_t *EA = NULL, *EB = NULL, *EC = NULL;
static idx_t ecap = 0;   /* arrays have ecap+3 slots */
static idx_t ehave = 0;  /* B[0..ehave-1] computed */

static void euler_extend(idx_t M) {
    if (M < ehave) return;
    if (M + 1 > ecap) {
        idx_t nc = ecap * 2;
        if (nc < M + 1) nc = M + 1;
        if (nc < 64) nc = 64;
        EA = grow_zero(EA, ecap ? ecap + 3 : 0, nc + 3);
        EB = grow_zero(EB, ecap ? ecap + 3 : 0, nc + 3);
        EC = grow_zero(EC, ecap ? ecap + 3 : 0, nc + 3);
        ecap = nc;
    }
    EA[1] = 1;
    for (idx_t m = ehave; m <= M; m++) {
        if (m == 0) {
            EB[0] = 1;
        } else {
            seq_t c = 0;
            for (idx_t d = 1; d * d <= m; d++) {
                if (m % d == 0) {
                    c += EA[d] * (seq_t)d;
                    idx_t e = m / d;
                    if (e != d) c += EA[e] * (seq_t)e;
                }
            }
            EC[m] = c;
            seq_t s = 0;
            for (idx_t k = 1; k <= m; k++) s += EC[k] * EB[m - k];
            EB[m] = s / (seq_t)m;
        }
        EA[m + 2] = EB[m];
    }
    ehave = M + 1;
}

static seq_t fixed_width(idx_t N) {
    seq_t *P = alloc_zero(N + 1);
    seq_t *Ct = alloc_zero(N + 2);
    seq_t res;
    P[0] = 1;
    for (idx_t k = 1; k <= N; k++) {
        seq_t ak = (k == 1) ? (seq_t)1 : P[k - 2];
        if (ak == 0) continue;
        idx_t T = N / k;
        Ct[0] = 1;
        for (idx_t t = 1; t <= T; t++) {
            seq_t g = gcd_seq(Ct[t - 1], (seq_t)t);
            if (g == 0) g = 1;
            seq_t tg = (seq_t)t / g;
            Ct[t] = (Ct[t - 1] / g) * ((ak + (seq_t)(t - 1)) / tg);
        }
        for (idx_t i = N; i >= k; i--) {
            seq_t s = 0;
            idx_t t = 1;
            for (idx_t j = k; j <= i; j += k, t++) s += Ct[t] * P[i - j];
            P[i] += s;
        }
    }
    res = P[N];
    release(P, N + 1);
    release(Ct, N + 2);
    return res;
}

seq_t a(idx_t n) {
    if (n <= 2) return 1;
    idx_t N = n - 2;
    if (seq_is_big()) {
        euler_extend(N);
        return EB[N];
    }
    return fixed_width(N);
}
