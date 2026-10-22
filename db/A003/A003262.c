/*
 * A003262  Let y=f(x) satisfy F(x,y)=0. a(n) is the number of terms in the
 *          expansion of (d/dx)^n y in terms of the partial derivatives of F.
 * https://oeis.org/A003262
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = [t^n u^(n-1)] Product_{i,j>=0,(i,j)<>(0,0),(0,1)} 1/(1 -
 *            t^i*u^(i+j-1)). Each factor is a partial derivative F_{x^i y^j};
 *            F itself (i=j=0) never occurs. Write s = j-1 (the "charge") and
 *            v = t*u. Then a(n) = [v^n] P(v) * Sum_{E>=0} PA_{E+1}(v) *
 *            C_E(v), where PA_k = partitions into exactly...
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 35, u64 n <= 94, u128 n <= 263,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a3262_ans = NULL;
static idx_t a3262_have = 0;

static void a3262_build(idx_t N) {
    idx_t E, m, k, a, x, w;
    seq_t **C = (seq_t **)malloc(N * sizeof(seq_t *));
    seq_t *acc = (seq_t *)calloc(N + 1, sizeof(seq_t));
    seq_t *S = (seq_t *)calloc(N + 1, sizeof(seq_t));
    seq_t *P = (seq_t *)calloc(N + 1, sizeof(seq_t));
    seq_t *prev = (seq_t *)calloc(N + 1, sizeof(seq_t));
    seq_t *cur = (seq_t *)calloc(N + 1, sizeof(seq_t));
    seq_t *tmp;
    seq_t *res = (seq_t *)calloc(N + 1, sizeof(seq_t));

    for (E = 0; E < N; E++) C[E] = (seq_t *)calloc(N - E, sizeof(seq_t));
    C[0][0] = 1;

    /* multiply by prod_{i>=0} 1/(1 - v^i z^m) = sum_k z^{mk}/(v;v)_k, Horner per row */
    for (m = 1; m < N; m++) {
        for (E = N - 1; E >= m; E--) {
            idx_t L = N - E, K = E / m;
            for (a = 0; a < L; a++) acc[a] = C[E - m * K][a];
            for (k = K; k >= 1; k--) {
                idx_t r = E - m * (k - 1);
                for (a = k; a < L; a++) acc[a] += acc[a - k];
                for (a = 0; a < L; a++) acc[a] += C[r][a];
            }
            for (a = 0; a < L; a++) C[E][a] = acc[a];
        }
    }

    /* PA_k rows rolled; S = sum_E PA_{E+1} * C_E; P = sum_k PA_k */
    prev[0] = 1;
    P[0] = 1;
    for (k = 1; k <= N; k++) {
        for (x = 0; x <= N; x++) {
            seq_t val = 0;
            if (x >= 1) val += prev[x - 1];
            if (x >= k) val += cur[x - k];
            cur[x] = val;
        }
        for (x = k; x <= N; x++) P[x] += cur[x];
        E = k - 1;
        if (E < N) {
            for (a = 0; a + k <= N; a++) {
                seq_t c = C[E][a];
                if (c == 0) continue;
                for (x = k; a + x <= N; x++) S[a + x] += c * cur[x];
            }
        }
        tmp = prev; prev = cur; cur = tmp;
    }

    for (x = 1; x <= N; x++) {
        seq_t s = 0;
        for (w = 1; w <= x; w++) s += P[x - w] * S[w];
        res[x] = s;
    }

    for (E = 0; E < N; E++) free(C[E]);
    free(C); free(acc); free(S); free(P); free(prev); free(cur);
    if (a3262_ans) free(a3262_ans);
    a3262_ans = res;
    a3262_have = N;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n > a3262_have) {
        idx_t N = a3262_have + a3262_have / 4;
        if (N < n) N = n;
        if (N < 16) N = 16;
        a3262_build(N);
    }
    return a3262_ans[n];
}
