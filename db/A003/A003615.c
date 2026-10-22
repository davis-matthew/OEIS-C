/*
 * A003615  Symmetries in planted 4-trees on n+1 vertices.
 * https://oeis.org/A003615
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum |Aut(T)| over rooted trees T on n vertices with at
 *            most 3 children per vertex (i.e. planted 4-trees on n+1
 *            vertices). Let S_p(x) = Sum_T |Aut(T)|^p x^|T|. Then S_p(x) = x
 *            * [y^0+y^1+y^2+y^3] Prod_{s>=1} (1 + W1(s) y x^s + W2(s) y^2
 *            x^(2s) + W3(s) y^3 x^(3s)), where, with P=[x^s]S_p, Q=...
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 23, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   alloc_zero, binom3, release, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/seq_is_fixed_width.h"
#include "../utils/binom3.h"

static seq_t *cache = NULL;
static idx_t cacheN = 0;

static seq_t binom2(seq_t P) {
    seq_t x, y;
    if (P < 2) return 0;
    x = P; y = P - 1;
    if (x % 2 == 0) x /= 2; else y /= 2;
    return x * y;
}

static void compute(idx_t N) {
    idx_t cnt = 0, cap = 0, i, j, k, t2, t;
    idx_t *qs, *Ms;
    seq_t **S;
    seq_t *H0, *H1, *H2, *H3;

    for (t2 = 1; t2 <= N; t2 *= 2)
        for (t = t2; t <= N; t *= 3) cap++;
    qs = (idx_t *)malloc(cap * sizeof(idx_t));
    Ms = (idx_t *)malloc(cap * sizeof(idx_t));
    S = (seq_t **)malloc(cap * sizeof(seq_t *));
    for (t2 = 1; t2 <= N; t2 *= 2)
        for (t = t2; t <= N; t *= 3) qs[cnt++] = t;
    for (i = 1; i < cnt; i++) {
        idx_t v = qs[i];
        j = i;
        while (j > 0 && qs[j - 1] < v) { qs[j] = qs[j - 1]; j--; }
        qs[j] = v;
    }

    H0 = alloc_zero(N + 1);
    H1 = alloc_zero(N + 1);
    H2 = alloc_zero(N + 1);
    H3 = alloc_zero(N + 1);

    for (k = 0; k < cnt; k++) {
        idx_t q = qs[k], M = N / q, d, e;
        seq_t *Sq, *S2 = NULL, *S3 = NULL;
        seq_t c2 = 1, c6 = 1;
        Ms[k] = M;
        S[k] = alloc_zero(M + 1);
        Sq = S[k];
        for (j = 0; j < k; j++) {
            if (qs[j] == 2 * q) S2 = S[j];
            if (qs[j] == 3 * q) S3 = S[j];
        }
        if (M >= 3)
            for (i = 0; i < q; i++) { c2 *= 2; c6 *= 6; }
        for (i = 0; i < M; i++) { H0[i] = 0; H1[i] = 0; H2[i] = 0; H3[i] = 0; }
        H0[0] = 1;

        for (d = 1; d <= M; d++) {
            idx_t s = d;
            seq_t P, W1, W2 = 0, W3 = 0;
            Sq[d] = H0[d - 1] + H1[d - 1] + H2[d - 1] + H3[d - 1];
            if (s >= M) break;
            P = Sq[s];
            W1 = P;
            if (2 * s + 1 <= M) {
                seq_t Q = S2[s];
                seq_t U = (Q - P) / 2;
                W2 = binom2(P) + c2 * Q - U;
                if (3 * s + 1 <= M) {
                    seq_t R = S3[s];
                    seq_t V = (R - P) / 6;
                    W3 = binom3(P) + 2 * V + c2 * (P * Q - R) + c6 * R - P * U;
                }
            }
            for (e = M - 1; e >= s; e--) {
                H3[e] += W1 * H2[e - s];
                if (e >= 2 * s) H3[e] += W2 * H1[e - 2 * s];
                if (e >= 3 * s) H3[e] += W3 * H0[e - 3 * s];
                H2[e] += W1 * H1[e - s];
                if (e >= 2 * s) H2[e] += W2 * H0[e - 2 * s];
                H1[e] += W1 * H0[e - s];
            }
        }
    }

    release(H0, N + 1);
    release(H1, N + 1);
    release(H2, N + 1);
    release(H3, N + 1);

    if (cache != NULL) release(cache, cacheN + 1);
    for (k = 0; k < cnt; k++) {
        if (qs[k] == 1) cache = S[k];
        else release(S[k], Ms[k] + 1);
    }
    cacheN = N;
    free(qs);
    free(Ms);
    free(S);
}

seq_t a(idx_t n) {
    if (n > cacheN) {
        idx_t grow = 2 * cacheN;
        idx_t N;
        if (grow < 32) grow = 32;
        if (seq_is_fixed_width() && grow > 256) grow = 0;
        N = n > grow ? n : grow;
        compute(N);
    }
    return cache[n];
}
