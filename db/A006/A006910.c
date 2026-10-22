/*
 * A006910  Theta series of laminated lattice LAMBDA_11^{min}.
 * https://oeis.org/A006910
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f. = theta_3(q^2)^11 + 26*theta_3(q^2)^7*theta_2(q^2)^4 +
 *            24*theta_3(q^2)^5*theta_2(q^2)^6 +
 *            13*theta_3(q^2)^3*theta_2(q^2)^8 -
 *            (1/2)*theta_4(q)^8*(theta_3(q^2)^3 - theta_4(q^2)^3), with
 *            theta_2(q^2) = 2*q^(1/2)*Sum_{k>=0} q^(2k(k+1)). Series built by
 *            sparse multiplications
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 75, u64 n <= 9253,
 *            u128 n <= 9253, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

typedef __int128 cf_t;

static cf_t *res_cache = NULL;
static idx_t res_len = 0;

/* sparse series: exponents and coefficients */
static idx_t *sp_e[4];
static int *sp_c[4];
static idx_t sp_n[4];

static void build_sparse(idx_t N) {
    int t;
    for (t = 0; t < 4; t++) {
        free(sp_e[t]); free(sp_c[t]);
        sp_e[t] = (idx_t *)malloc(sizeof(idx_t) * (2 * N + 4));
        sp_c[t] = (int *)malloc(sizeof(int) * (2 * N + 4));
        sp_n[t] = 0;
    }
    /* 0: theta_3(q^2), 1: theta_4(q^2), 2: theta_4(q), 3: s(q)=Sum q^(2k(k+1)) */
    sp_e[0][0] = 0; sp_c[0][0] = 1; sp_n[0] = 1;
    sp_e[1][0] = 0; sp_c[1][0] = 1; sp_n[1] = 1;
    sp_e[2][0] = 0; sp_c[2][0] = 1; sp_n[2] = 1;
    {
        idx_t k;
        for (k = 1; 2 * k * k < N; k++) {
            sp_e[0][sp_n[0]] = 2 * k * k; sp_c[0][sp_n[0]++] = 2;
            sp_e[1][sp_n[1]] = 2 * k * k; sp_c[1][sp_n[1]++] = (k & 1) ? -2 : 2;
        }
        for (k = 1; k * k < N; k++) {
            sp_e[2][sp_n[2]] = k * k; sp_c[2][sp_n[2]++] = (k & 1) ? -2 : 2;
        }
        for (k = 0; 2 * k * (k + 1) < N; k++) {
            sp_e[3][sp_n[3]] = 2 * k * (k + 1); sp_c[3][sp_n[3]++] = 1;
        }
    }
}

static cf_t *smul(cf_t *x, idx_t N, int t) {
    cf_t *y = (cf_t *)calloc(N, sizeof(cf_t));
    idx_t j, i;
    for (j = 0; j < sp_n[t]; j++) {
        idx_t e = sp_e[t][j];
        cf_t c = sp_c[t][j];
        for (i = e; i < N; i++) y[i] += c * x[i - e];
    }
    free(x);
    return y;
}

static cf_t *delta(idx_t N) {
    cf_t *d = (cf_t *)calloc(N, sizeof(cf_t));
    d[0] = 1;
    return d;
}

static void compute(idx_t N) {
    cf_t *S, *S4 = NULL, *S6 = NULL, *I, *T, *E1, *E2;
    idx_t i;
    int m;
    build_sparse(N);
    S = delta(N);
    for (m = 1; m <= 8; m++) {
        S = smul(S, N, 3);
        if (m == 4) { S4 = (cf_t *)malloc(N * sizeof(cf_t)); for (i = 0; i < N; i++) S4[i] = S[i]; }
        if (m == 6) { S6 = (cf_t *)malloc(N * sizeof(cf_t)); for (i = 0; i < N; i++) S6[i] = S[i]; }
    }
    I = delta(N);
    for (m = 0; m < 4; m++) I = smul(I, N, 0);
    for (i = 2; i < N; i++) I[i] += 416 * S4[i - 2];
    for (m = 0; m < 2; m++) I = smul(I, N, 0);
    for (i = 3; i < N; i++) I[i] += 1536 * S6[i - 3];
    for (m = 0; m < 2; m++) I = smul(I, N, 0);
    for (i = 4; i < N; i++) I[i] += 3328 * S[i - 4];
    for (m = 0; m < 3; m++) I = smul(I, N, 0);

    T = delta(N);
    for (m = 0; m < 8; m++) T = smul(T, N, 2);
    E1 = (cf_t *)malloc(N * sizeof(cf_t));
    E2 = (cf_t *)malloc(N * sizeof(cf_t));
    for (i = 0; i < N; i++) { E1[i] = T[i]; E2[i] = T[i]; }
    for (m = 0; m < 3; m++) { E1 = smul(E1, N, 0); E2 = smul(E2, N, 1); }
    for (i = 0; i < N; i++) I[i] -= (E1[i] - E2[i]) / 2;

    free(S); free(S4); free(S6); free(T); free(E1); free(E2);
    free(res_cache);
    res_cache = I;
    res_len = N;
}

static seq_t to_seq(cf_t v) {
    unsigned __int128 u = (unsigned __int128)v;
    uint64_t hi = (uint64_t)(u >> 64), lo = (uint64_t)u;
    seq_t r;
    if (hi == 0) return (seq_t)lo;
    r = (seq_t)hi;
    r <<= 32;
    r <<= 32;
    r += (seq_t)lo;
    return r;
}

seq_t a(idx_t n) {
    if (n >= res_len) {
        idx_t N = 2 * res_len;
        if (N < n + 1) N = n + 1;
        if (N < 64) N = 64;
        compute(N);
    }
    return to_seq(res_cache[n]);
}
