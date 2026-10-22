/*
 * A007257  McKay-Thompson series of class 6D for Monster.
 * https://oeis.org/A007257
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = [q^(n+1)] (eta(q)*eta(q^2)/(eta(q^3)*eta(q^6)))^4, plus 4
 *            when n = 0; each eta factor is expanded by Euler's pentagonal
 *            number theorem and applied as a sparse multiplication or
 *            division of the power series
 * Range:     i8 n <= 4, i16 n <= 19, i32 n <= 73, i64 n <= 265,
 *            i128 n <= 1000, big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static seq_t *F = NULL;
static idx_t Flen = 0;

static idx_t *pe = NULL;
static int *ps = NULL;
static idx_t pcnt = 0;

static void build_pent(idx_t N) {
    idx_t j, k = 0;
    idx_t maxc = 2;
    for (j = 1; j * (3 * j - 1) / 2 <= N; j++) maxc += 2;
    pe = (idx_t *)malloc(sizeof(idx_t) * (size_t)maxc);
    ps = (int *)malloc(sizeof(int) * (size_t)maxc);
    for (j = 1;; j++) {
        idx_t e1 = j * (3 * j - 1) / 2;
        idx_t e2 = j * (3 * j + 1) / 2;
        int s = (j & 1) ? -1 : 1;
        if (e1 > N) break;
        pe[k] = e1; ps[k] = s; k++;
        if (e2 <= N) { pe[k] = e2; ps[k] = s; k++; }
    }
    pcnt = k;
}

/* f *= E(q^m), where E(q) = prod (1-q^k) = 1 + sum ps[t] q^pe[t] */
static void mul_eta(seq_t *f, idx_t N, idx_t m) {
    idx_t i, t;
    for (i = N; i >= 1; i--) {
        seq_t s = 0;
        for (t = 0; t < pcnt; t++) {
            idx_t e = m * pe[t];
            if (e > i) break;
            if (ps[t] > 0) s += f[i - e]; else s -= f[i - e];
        }
        f[i] += s;
    }
}

/* f /= E(q^m) */
static void div_eta(seq_t *f, idx_t N, idx_t m) {
    idx_t i, t;
    for (i = 1; i <= N; i++) {
        seq_t s = 0;
        for (t = 0; t < pcnt; t++) {
            idx_t e = m * pe[t];
            if (e > i) break;
            if (ps[t] > 0) s += f[i - e]; else s -= f[i - e];
        }
        f[i] -= s;
    }
}

static void build(idx_t N) {
    int r;
    seq_t *f = alloc_zero(N + 1);
    f[0] = 1;
    build_pent(N);
    for (r = 0; r < 4; r++) {
        mul_eta(f, N, 1);
        mul_eta(f, N, 2);
        div_eta(f, N, 3);
        div_eta(f, N, 6);
    }
    free(pe); pe = NULL;
    free(ps); ps = NULL;
    if (F) release(F, Flen);
    F = f;
    Flen = N + 1;
}

seq_t a(idx_t n) {
    idx_t need;
    seq_t r;
    if (n < -1) return 0;
    need = n + 1;
    if (need >= Flen) {
        idx_t N = need;
        if (N < 2 * Flen) N = 2 * Flen;
        if (N < 64) N = 64;
        build(N);
    }
    r = F[need];
    if (n == 0) r += 4;
    return r;
}
