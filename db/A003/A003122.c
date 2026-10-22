/*
 * A003122  Number of Hamiltonian rooted triangulations with n internal nodes
 *          and 3 external nodes.
 * https://oeis.org/A003122
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   let G(x) = Sum_{i>=0} r(i)*x^i with r(i) =
 *            (binomial(2*i,i)/(i+1))^2. Then B(s,m) = [x^s] (G(x)-1)^m and
 *            A(n,s) = [x^s] G(x)^n, so the recurrence f(n,k) = p(n,k) -
 *            Sum_{s=0..n-1} f(s,k)*A(k+s,n-s) reads Sum_{s=0..n} f(s,3) *
 *            [x^n] x^s G(x)^(3+s) = p(n,3), with p(n,3) =
 *            3*((2n+2)!)^2/(n!(n+1)!(n+2...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 36,
 *            big any n
 * Helpers:   h3122_arr, h3122_fixed, h3122_inv, h3122_mulf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/h3122_arr.h"
#include "../utils/h3122_fixed.h"
#include "../utils/h3122_inv.h"
#include "../utils/h3122_mulf.h"

static seq_t *h3122_f = NULL;
static idx_t h3122_have = 0;

static void h3122_compute(idx_t N) {
    idx_t i, j, m, s;
    int fixed = h3122_fixed();
    seq_t *P = h3122_arr(N + 1);
    seq_t *C = h3122_arr(N + 1);
    seq_t *G = h3122_arr(N + 1);
    seq_t *G2 = h3122_arr(N + 1);
    seq_t *T = h3122_arr(N + 1);
    seq_t *F = h3122_arr(N + 1);

    /* p(m,3) */
    P[0] = 1;
    if (fixed) {
        seq_t O = 1;
        long e = 0;
        for (m = 1; m <= N; m++) {
            seq_t pw = 1;
            long q;
            h3122_mulf(&O, &e, 2 * m + 1, 1);
            h3122_mulf(&O, &e, 2 * m + 1, 1);
            h3122_mulf(&O, &e, 2 * m + 2, 1);
            h3122_mulf(&O, &e, 2 * m + 2, 1);
            h3122_mulf(&O, &e, m, -1);
            h3122_mulf(&O, &e, m + 1, -1);
            h3122_mulf(&O, &e, m + 2, -1);
            h3122_mulf(&O, &e, m + 3, -1);
            for (q = 0; q < e && pw != 0; q++) pw = pw + pw;
            P[m] = O * pw;
        }
    } else {
        for (m = 1; m <= N; m++) {
            seq_t num = P[m - 1] * (seq_t)(2 * m + 1) * (seq_t)(2 * m + 1)
                        * (seq_t)(2 * m + 2) * (seq_t)(2 * m + 2);
            seq_t den = (seq_t)m * (seq_t)(m + 1) * (seq_t)(m + 2) * (seq_t)(m + 3);
            P[m] = num / den;
        }
    }

    /* Catalan numbers and G */
    C[0] = 1;
    for (i = 1; i <= N; i++) {
        seq_t acc = 0;
        for (j = 0; j < i; j++) acc += C[j] * C[i - 1 - j];
        C[i] = acc;
    }
    for (i = 0; i <= N; i++) G[i] = C[i] * C[i];

    /* T = G^3 */
    for (i = 0; i <= N; i++) {
        seq_t acc = 0;
        for (j = 0; j <= i; j++) acc += G[j] * G[i - j];
        G2[i] = acc;
    }
    for (i = 0; i <= N; i++) {
        seq_t acc = 0;
        for (j = 0; j <= i; j++) acc += G2[j] * G[i - j];
        T[i] = acc;
    }

    /* residual R = P (reuse P) */
    for (s = 0; s <= N; s++) {
        seq_t fs = P[s];
        F[s] = fs;
        if (s == N) break;
        for (m = s + 1; m <= N; m++) P[m] -= fs * T[m];
        /* T <- x*G*T */
        for (m = N; m > s; m--) {
            seq_t acc = 0;
            for (j = s; j < m; j++) acc += T[j] * G[m - 1 - j];
            T[m] = acc;
        }
        T[s] = 0;
    }

    free(P); free(C); free(G); free(G2); free(T);
    if (h3122_f) free(h3122_f);
    h3122_f = F;
    h3122_have = N + 1;
}

seq_t a(idx_t n) {
    if (n >= h3122_have) {
        idx_t N = n + n / 4;
        if (N < 32) N = 32;
        h3122_compute(N);
    }
    return h3122_f[n];
}
