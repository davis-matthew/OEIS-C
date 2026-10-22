/*
 * A003483  Number of square permutations of n elements.
 * https://oeis.org/A003483
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = Sum_{j=0..n, 4|j} binomial(n,j) * o(n-j) * e(j), where
 *            o(0) = 1, o(i) = i*o(i-1) for odd i, o(i) = (i-1)*o(i-1) for
 *            even i (permutations with only odd cycles, e.g.f.
 *            sqrt((1+x)/(1-x))), and e(j) counts permutations with only even
 *            cycles, each even length occurring an even number of times (...
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   A_grow, off
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/A_grow.h"
#include "../utils/off.h"

static seq_t *E = NULL;      /* E[off(t)+k] = E_k(4t), k = 0..t */
static seq_t *S = NULL;      /* S[off(t)+k] = P_k(4t) when k | t */
static idx_t cap = 0;
static seq_t *brow = NULL;   /* brow[q] = binomial(4t, 4q) */
static idx_t bcap = 0;
static idx_t rows = 0;       /* rows 0..rows-1 are computed */

static void ensure(idx_t T) {
    idx_t need = off(T + 1);
    if (need > cap) {
        idx_t nc = cap * 2;
        if (nc < need) nc = need;
        E = A_grow(E, cap, nc);
        S = A_grow(S, cap, nc);
        cap = nc;
    }
    if (T + 1 > bcap) {
        idx_t nb = bcap * 2;
        if (nb < T + 1) nb = T + 1;
        brow = A_grow(brow, bcap, nb);
        bcap = nb;
    }
    for (idx_t t = rows; t <= T; t++) {
        idx_t base = off(t);
        idx_t j = 4 * t;
        S[base] = (t == 0) ? (seq_t)1 : (seq_t)0;
        E[base] = (t == 0) ? (seq_t)1 : (seq_t)0;
        for (idx_t k = 1; k <= t; k++) {
            if (t % k != 0) { S[base + k] = 0; continue; }
            idx_t m = t / k;
            seq_t v = (m == 1) ? (seq_t)1 : S[off(t - k) + k];
            idx_t x = j - 2 * k - 1;
            for (idx_t r = 0; r < 2 * k - 1; r++) v = v * (seq_t)(x - r);
            x = j - 1;
            for (idx_t r = 0; r < 2 * k - 1; r++) v = v * (seq_t)(x - r);
            S[base + k] = v;
        }
        seq_t c = 1;
        for (idx_t i = 0; i <= j; i++) {
            if (i % 4 == 0) brow[i / 4] = c;
            if (i < j) c = c * (seq_t)(j - i) / (seq_t)(i + 1);
        }
        for (idx_t k = 1; k <= t; k++) {
            seq_t sum = 0;
            for (idx_t m = 0; k * m <= t; m++) {
                idx_t q = k * m;
                idx_t tp = t - q;
                idx_t ki = (k - 1 < tp) ? k - 1 : tp;
                seq_t ev = E[off(tp) + ki];
                if (ev == 0) continue;
                seq_t sp = (m == 0) ? (seq_t)1 : S[off(q) + k];
                sum += brow[q] * ev * sp;
            }
            E[base + k] = sum;
        }
    }
    if (T + 1 > rows) rows = T + 1;
}

seq_t a(idx_t n) {
    idx_t T = n / 4;
    if (T + 1 > rows) ensure(T);
    seq_t total = 0;
    seq_t c = 1;   /* binomial(n, i) */
    seq_t ov = 1;  /* o(i) */
    for (idx_t i = 0; i <= n; i++) {
        idx_t jj = n - i;
        if (jj % 4 == 0) {
            idx_t t = jj / 4;
            total += c * ov * E[off(t) + t];
        }
        if (i < n) {
            c = c * (seq_t)(n - i) / (seq_t)(i + 1);
            if ((i + 1) % 2 == 1) ov = ov * (seq_t)(i + 1);
            else ov = ov * (seq_t)i;
        }
    }
    return total;
}
