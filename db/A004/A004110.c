/*
 * A004110  Number of n-node unlabeled graphs without endpoints (i.e., no
 *          nodes of degree 1).
 * https://oeis.org/A004110
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [x^n] Z(G; x_k -> x^k - x^(2k)), the cycle index of
 *            graphs with each x_k replaced by x^k(1-x^k): a(n) = Sum_{m<=n}
 *            Sum_{mu partition of m} 2^E(mu)/z(mu) * [x^(n-m)] Prod_{k}
 *            (1-x^k)^{c_k(mu)}, where E(mu) = Sum_k (c_k*floor(k/2) +
 *            k*c_k*(c_k-1)/2) + Sum_{i<j} gcd(i,j)*c_i*c_j (sum over pai...
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 11, u64 n <= 14, u128 n <= 19,
 *            big any n
 * Helpers:   bn_div
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/bn_div.h"

static int g_n, g_ls;
static uint32_t *g_w, *g_s, *g_tmp;
static __int128 *g_p;
static int *g_cnt;

static int g_gcd(int a, int b) { while (b) { int t = a % b; a = b; b = t; } return a; }

static void bn_mul(uint32_t *a, int L, uint32_t d) {
    uint64_t carry = 0;
    for (int i = 0; i < L; i++) {
        uint64_t cur = (uint64_t)a[i] * d + carry;
        a[i] = (uint32_t)cur;
        carry = cur >> 32;
    }
}

static void dfs(int depth, int m, long long e, int maxp) {
    int N = g_n, LS = g_ls;
    uint32_t *W = g_w + (size_t)depth * LS;
    __int128 *P = g_p + (size_t)depth * (N + 1);
    __int128 coef = P[N - m];
    if (coef != 0) {
        bool neg = coef < 0;
        unsigned __int128 u = neg ? (unsigned __int128)(-coef) : (unsigned __int128)coef;
        memset(g_tmp, 0, sizeof(uint32_t) * LS);
        for (int j = 0; j < 4 && u != 0; j++, u >>= 32) {
            uint32_t c = (uint32_t)u;
            if (!c) continue;
            uint64_t carry = 0;
            for (int i = 0; i + j < LS; i++) {
                uint64_t cur = (uint64_t)g_tmp[i + j] + (uint64_t)W[i] * c + carry;
                g_tmp[i + j] = (uint32_t)cur;
                carry = cur >> 32;
            }
        }
        uint32_t *S = g_s + (size_t)e * LS;
        if (!neg) {
            uint64_t carry = 0;
            for (int i = 0; i < LS; i++) {
                uint64_t cur = (uint64_t)S[i] + g_tmp[i] + carry;
                S[i] = (uint32_t)cur;
                carry = cur >> 32;
            }
        } else {
            int64_t borrow = 0;
            for (int i = 0; i < LS; i++) {
                int64_t cur = (int64_t)S[i] - (int64_t)g_tmp[i] - borrow;
                borrow = cur < 0;
                S[i] = (uint32_t)(cur + (borrow ? ((int64_t)1 << 32) : 0));
            }
        }
    }
    int top = maxp < N - m ? maxp : N - m;
    for (int k = top; k >= 1; k--) {
        long long de = k / 2;
        for (int l = k; l <= N; l++)
            if (g_cnt[l]) de += (long long)g_gcd(k, l) * g_cnt[l];
        g_cnt[k]++;
        uint32_t *W2 = g_w + (size_t)(depth + 1) * LS;
        memcpy(W2, W, sizeof(uint32_t) * LS);
        bn_div(W2, LS, (uint32_t)k);
        bn_div(W2, LS, (uint32_t)g_cnt[k]);
        __int128 *P2 = g_p + (size_t)(depth + 1) * (N + 1);
        int m2 = m + k, D = N - m2;
        for (int d = 0; d <= D; d++)
            P2[d] = P[d] - (d >= k ? P[d - k] : 0);
        dfs(depth + 1, m2, e + de, k);
        g_cnt[k]--;
    }
}

static seq_t compute(int N) {
    long long Emax = (long long)N * (N - 1) / 2;
    long long bits = N + 40;
    for (int t = 2; t <= N + 1; t++) { int b = 0, x = t; while (x) { b++; x >>= 1; } bits += b; }
    int LS = (int)(bits / 32) + 1;
    g_n = N; g_ls = LS;
    g_w = (uint32_t *)calloc((size_t)(N + 2) * LS, sizeof(uint32_t));
    g_s = (uint32_t *)calloc((size_t)(Emax + 1) * LS, sizeof(uint32_t));
    g_tmp = (uint32_t *)calloc((size_t)LS, sizeof(uint32_t));
    g_p = (__int128 *)calloc((size_t)(N + 2) * (N + 1), sizeof(__int128));
    g_cnt = (int *)calloc((size_t)N + 2, sizeof(int));
    g_w[0] = 1;
    for (int t = 2; t <= N; t++) bn_mul(g_w, LS, (uint32_t)t);
    g_p[0] = 1;
    dfs(0, 0, 0, N);

    int LA = LS + (int)(Emax / 32) + 2;
    uint32_t *A = (uint32_t *)calloc((size_t)LA, sizeof(uint32_t));
    for (long long E = 0; E <= Emax; E++) {
        uint32_t *S = g_s + (size_t)E * LS;
        bool nz = false;
        for (int i = 0; i < LS; i++) if (S[i]) { nz = true; break; }
        if (!nz) continue;
        uint32_t fill = (S[LS - 1] & 0x80000000u) ? 0xFFFFFFFFu : 0;
        int q = (int)(E / 32), b = (int)(E % 32);
        uint64_t carry = 0;
        for (int i = q; i < LA; i++) {
            int j = i - q;
            uint32_t cj = j < LS ? S[j] : fill;
            uint32_t cp = j == 0 ? 0 : (j - 1 < LS ? S[j - 1] : fill);
            uint32_t v = b ? ((cj << b) | (cp >> (32 - b))) : cj;
            uint64_t cur = (uint64_t)A[i] + v + carry;
            A[i] = (uint32_t)cur;
            carry = cur >> 32;
        }
    }
    for (int t = 2; t <= N; t++) bn_div(A, LA, (uint32_t)t);

    seq_t r = 0;
    int i = LA * 4 - 1;
    while (i > 0 && ((A[i / 4] >> (8 * (i % 4))) & 0xFF) == 0) i--;
    for (; i >= 0; i--) {
        int byte = (int)((A[i / 4] >> (8 * (i % 4))) & 0xFF);
        r *= 256;
        r += byte;
    }
    free(A); free(g_w); free(g_s); free(g_tmp); free(g_p); free(g_cnt);
    return r;
}

#define A004110_MEMO 256
static seq_t a004110_memo[A004110_MEMO];
static bool a004110_have[A004110_MEMO];

seq_t a(idx_t n) {
    if (n < A004110_MEMO) {
        if (!a004110_have[n]) {
            a004110_memo[n] = compute((int)n);
            a004110_have[n] = true;
        }
        return a004110_memo[n];
    }
    return compute((int)n);
}
