/*
 * A005816  Number of 4-valent labeled graphs with n nodes where multiple
 *          edges and loops are allowed.
 * https://oeis.org/A005816
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   E.g.f. = (1-t/2)^(-1/2) * exp(t/4 + t^2/8 + t/(8*(1-t/2))) *
 *            Sum_{j,k>=0} (2j+4k-1)!!/(j!*k!) * u^j * v^k, u = t^2/6 +
 *            t/(4*(1-t/2)), v = t/24 + t^2/(16*(1-t/2)). This is the
 *            Gaussian-integral form of a(n) = N{E_n[S_4] * S_{2n}[S_2]}: the
 *            coefficient of prod x_i^4 in prod_i 1/(1-x_i^2) * prod_{i<...
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 14, u128 n <= 22,
 *            big any n
 * Helpers:   alloc_zero, release, seq_is_big, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/seq_is_big.h"
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/u128_to_seq.h"

/* ---------- small (fixed width) path: residual-degree DP ---------- */
typedef unsigned __int128 u128_t;
static const int PR[10] = {1, 2, 2, 3, 3, 3, 4, 4, 4, 4};
static const int PM[10] = {1, 1, 2, 1, 2, 3, 1, 2, 3, 4};
static unsigned long long sbc[32][32];

static void enum_rec(int p, int budget, const int *orig, int *used, int *nc,
                     u128_t ways, u128_t val, u128_t *nxt, int D) {
    if (p == 10) {
        int c[5];
        memcpy(c, nc, sizeof(c));
        if (budget > 0) c[budget]++;
        size_t idx = (((size_t)c[1] * D + c[2]) * D + c[3]) * D + c[4];
        nxt[idx] += val * ways;
        return;
    }
    int r = PR[p], m = PM[p];
    int avail = orig[r] - used[r];
    for (int k = 0; k <= avail && k * m <= budget; k++) {
        used[r] += k;
        nc[r] -= k;
        if (r - m > 0) nc[r - m] += k;
        enum_rec(p + 1, budget - k * m, orig, used, nc,
                 ways * (u128_t)sbc[avail][k], val, nxt, D);
        used[r] -= k;
        nc[r] += k;
        if (r - m > 0) nc[r - m] -= k;
    }
}

static seq_t small_dp(idx_t n) {
    if (n > 24) return 0; /* does not fit in any fixed width */
    int N = (int)n, D = N + 1;
    for (int i = 0; i < 32; i++) {
        sbc[i][0] = 1;
        for (int j = 1; j < 32; j++)
            sbc[i][j] = (i == 0) ? 0 : sbc[i - 1][j - 1] + sbc[i - 1][j];
    }
    size_t sz = (size_t)D * D * D * D;
    u128_t *cur = (u128_t *)calloc(sz, sizeof(u128_t));
    u128_t *nxt = (u128_t *)calloc(sz, sizeof(u128_t));
    cur[0] = 1;
    for (int step = 0; step < N; step++) {
        memset(nxt, 0, sz * sizeof(u128_t));
        for (int c1 = 0; c1 <= step; c1++)
            for (int c2 = 0; c1 + c2 <= step; c2++)
                for (int c3 = 0; c1 + c2 + c3 <= step; c3++)
                    for (int c4 = 0; c1 + c2 + c3 + c4 <= step; c4++) {
                        size_t idx = (((size_t)c1 * D + c2) * D + c3) * D + c4;
                        u128_t val = cur[idx];
                        if (val == 0) continue;
                        int orig[5] = {0, c1, c2, c3, c4};
                        for (int l = 0; l <= 2; l++) {
                            int used[5] = {0, 0, 0, 0, 0};
                            int nc[5] = {0, c1, c2, c3, c4};
                            enum_rec(0, 4 - 2 * l, orig, used, nc, 1, val, nxt, D);
                        }
                    }
        u128_t *t = cur; cur = nxt; nxt = t;
    }
    u128_t res = cur[0];
    free(cur);
    free(nxt);
    return u128_to_seq(res);
}

/* ---------- big path: series in y = t/2 ---------- */
static seq_t *gW = NULL, *gQ = NULL;
static idx_t gLen = 0;

static void build(idx_t M) {
    idx_t len = M + 1;
    seq_t *W = alloc_zero(len), *q = alloc_zero(len);
    q[0] = 1;
    for (idx_t m = 0; m + 1 < len; m++) {
        seq_t s = q[m] * (seq_t)(8 * m + 5);
        if (m >= 3) s += q[m - 3] * (seq_t)256 * (seq_t)m * (seq_t)(m - 1) * (seq_t)(m - 2);
        if (m >= 1) s -= q[m - 1] * (seq_t)(4 * m) * (seq_t)(4 * m - 2);
        if (m >= 2) s -= q[m - 2] * (seq_t)96 * (seq_t)m * (seq_t)(m - 1);
        q[m + 1] = s;
    }
    idx_t plen = 2 * M + 1;
    seq_t *P24 = alloc_zero(plen);
    P24[0] = 1;
    for (idx_t i = 1; i < plen; i++) P24[i] = P24[i - 1] * 24;
    seq_t *Hp = alloc_zero(len), *Hc = alloc_zero(len), *Hn = alloc_zero(len);
    Hc[0] = 1;
    seq_t cb = 1;
    for (idx_t N = 0; N <= 2 * M; N++) {
        for (idx_t m = (N + 1) / 2; m <= M; m++)
            W[m] += cb * Hc[m] * P24[2 * m - N];
        if (N == 2 * M) break;
        seq_t sc1 = 0, sc2 = 0, sp1 = 0, sp2 = 0;
        for (idx_t m = 0; m <= M; m++) {
            seq_t val = 0;
            if (m >= 2) val += Hc[m - 2] * 8;
            if (m >= 1) val += sc1 * 6;
            if (N >= 1) {
                seq_t t = 0;
                if (m >= 1) t += Hp[m - 1];
                if (m >= 2) t += sp2 * 3;
                val += t * (seq_t)(24 * N);
            }
            Hn[m] = val;
            sc2 = sc1; sc1 += Hc[m];
            sp2 = sp1; sp1 += Hp[m];
        }
        seq_t *tmp = Hp; Hp = Hc; Hc = Hn; Hn = tmp;
        cb = cb * (seq_t)(2 * (2 * N + 1)) / (seq_t)(N + 1);
    }
    release(Hp, len);
    release(Hc, len);
    release(Hn, len);
    release(P24, plen);
    if (gW) {
        release(gW, gLen);
        release(gQ, gLen);
    }
    gW = W;
    gQ = q;
    gLen = len;
}

seq_t a(idx_t n) {
    if (!seq_is_big()) return small_dp(n);
    if (n + 1 > gLen) {
        idx_t M = n;
        if (gLen > 0) {
            idx_t grow = (gLen - 1) + (gLen - 1) / 2;
            if (grow > M) M = grow;
        }
        build(M);
    }
    seq_t *pw = alloc_zero(n + 1);
    pw[0] = 1;
    for (idx_t i = 1; i <= n; i++) pw[i] = pw[i - 1] * 144;
    seq_t sum = 0, bin = 1, fact = 1;
    for (idx_t m = 0; m <= n; m++) {
        sum += bin * gQ[n - m] * fact * gW[m] * pw[n - m];
        bin = bin * (seq_t)(n - m) / (seq_t)(m + 1);
        fact *= (seq_t)(m + 1);
    }
    release(pw, n + 1);
    seq_t den = 1;
    for (idx_t i = 0; i < n; i++) den *= 1152;
    return sum / den;
}
