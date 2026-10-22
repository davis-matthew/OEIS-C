/*
 * A006265  Number of shapes of height-balanced AVL trees with n nodes.
 * https://oeis.org/A006265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A(x) = Sum_{h>=0} P_h(x), where P_0 = x, P_{-1} = 0, P_{h+1} =
 *            P_h^2 + 2*P_h*P_{h-1} (the unrolled form of B(x,y) = x +
 *            B(x^2+2xy, x)); a(n) = [x^n] A(x)
 * Range:     u8 n <= 13, u16 n <= 22, u32 n <= 40, u64 n <= 75, u128 n <= 144,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static seq_t *S_cache = 0;
static idx_t S_N = 0;

static void build(idx_t N) {
    idx_t L = N + 1;
    seq_t *s = alloc_zero(L);
    seq_t *p = alloc_zero(L);
    seq_t *c = alloc_zero(L);
    seq_t *q = alloc_zero(L);
    seq_t *t = alloc_zero(L);
    idx_t clo = 1, chi = 1, plo = 0;
    int pempty = 1;
    idx_t k, i, j;

    c[1] = 1;
    while (clo <= N) {
        idx_t qlo, qhi, nlo, nhi;
        seq_t *tmp;
        for (k = clo; k <= chi; k++) s[k] += c[k];

        qlo = pempty ? clo : plo;
        qhi = chi;
        for (k = qlo; k <= qhi; k++) q[k] = c[k] + 2 * p[k];

        nlo = clo + qlo;
        if (nlo > N) break;
        nhi = chi + qhi;
        if (nhi > N) nhi = N;

        for (k = 0; k <= N; k++) t[k] = 0;
        for (i = clo; i <= chi; i++) {
            idx_t jmax;
            if (c[i] == 0) continue;
            jmax = N - i;
            if (jmax > qhi) jmax = qhi;
            for (j = qlo; j <= jmax; j++) t[i + j] += c[i] * q[j];
        }

        for (k = 0; k <= N; k++) p[k] = 0;
        tmp = p;
        p = c;
        c = t;
        t = tmp;
        plo = clo;
        pempty = 0;
        clo = nlo;
        chi = nhi;
    }

    release(p, L);
    release(c, L);
    release(q, L);
    release(t, L);
    if (S_cache) release(S_cache, S_N + 1);
    S_cache = s;
    S_N = N;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (S_cache == 0 || n > S_N) build(n);
    return S_cache[n];
}
