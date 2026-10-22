/*
 * A000860  Number of n-input 3-output switching networks under action of S(n)
 *          and complementing group C(2,3) on inputs and outputs.
 * https://oeis.org/A000860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Burnside count. a(n) = (1/(48*2^n*n!)) * Sum_{g in B_n} Sum_{h
 *            in B_3} Prod_{cycles C of g on {0,1}^n} fix(h^|C|), where B_n =
 *            S(n) x C(2,n) permutes/complements the n inputs, B_3 = S(3) x
 *            C(2,3) the 3 outputs, and fix(h^L) is the number of points of
 *            {0,1}^3 fixed by h^L. Summed over signed cycle...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   a677_from_u128, a860_fixlog, a860_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/a677_from_u128.h"
#include "../utils/a860_fixlog.h"
#include "../utils/a860_gcd.h"

static int h_cnt;
static int h_k[16][3], h_p[16][3], h_nc[16];
static uint64_t h_size[16];

static int *g_data;
static long long *g_exp;
static int g_cnt, g_cap, g_stride;

static int cur_k[64], cur_p[64], cur_nc;
static int cur_mode;

static void a860_process_h(void) {
    uint64_t z = 1;
    int run = 0;
    for (int i = 0; i < cur_nc; i++) {
        if (i > 0 && cur_k[i] == cur_k[i - 1] && cur_p[i] == cur_p[i - 1]) run++; else run = 1;
        z *= (uint64_t)(2 * cur_k[i]) * (uint64_t)run;
        h_k[h_cnt][i] = cur_k[i];
        h_p[h_cnt][i] = cur_p[i];
    }
    h_nc[h_cnt] = cur_nc;
    h_size[h_cnt] = 48 / z;
    h_cnt++;
}

static void a860_process_g(void) {
    uint64_t order = 1;
    for (int i = 0; i < cur_nc; i++) {
        uint64_t c = (uint64_t)(cur_p[i] ? 2 * cur_k[i] : cur_k[i]);
        order = order / a860_gcd(order, c) * c;
    }
    int nd = 0, cap = 64;
    uint64_t *D = (uint64_t *)malloc((size_t)cap * sizeof(uint64_t));
    for (uint64_t d = 1; d * d <= order; d++) {
        if (order % d) continue;
        if (nd + 2 > cap) { cap *= 2; D = (uint64_t *)realloc(D, (size_t)cap * sizeof(uint64_t)); }
        D[nd++] = d;
        if (d != order / d) D[nd++] = order / d;
    }
    for (int i = 1; i < nd; i++) {
        uint64_t v = D[i];
        int j = i;
        while (j > 0 && D[j - 1] > v) { D[j] = D[j - 1]; j--; }
        D[j] = v;
    }
    long long *ex = (long long *)malloc((size_t)nd * sizeof(long long));
    for (int i = 0; i < nd; i++) {
        long long v = a860_fixlog(cur_k, cur_p, cur_nc, D[i]);
        long long val = v < 0 ? 0 : (1LL << v);
        for (int j = 0; j < i; j++)
            if (D[i] % D[j] == 0) val -= ex[j];
        ex[i] = val;
    }
    if (g_cnt >= g_cap) {
        g_cap = g_cap ? 2 * g_cap : 64;
        g_data = (int *)realloc(g_data, (size_t)g_cap * (size_t)g_stride * sizeof(int));
        g_exp = (long long *)realloc(g_exp, (size_t)g_cap * (size_t)h_cnt * sizeof(long long));
    }
    int *d = g_data + (size_t)g_cnt * g_stride;
    d[0] = cur_nc;
    for (int i = 0; i < cur_nc; i++) { d[1 + 2 * i] = cur_k[i]; d[2 + 2 * i] = cur_p[i]; }
    for (int j = 0; j < h_cnt; j++) {
        long long e = 0;
        for (int i = 0; i < nd && e >= 0; i++) {
            if (ex[i] <= 0) continue;
            long long m = ex[i] / (long long)D[i];
            long long fl = a860_fixlog(h_k[j], h_p[j], h_nc[j], D[i]);
            if (fl < 0) e = -1; else e += m * fl;
        }
        g_exp[(size_t)g_cnt * h_cnt + j] = e;
    }
    g_cnt++;
    free(ex);
    free(D);
}

static void a860_rec(int rem, int maxt) {
    if (rem == 0) {
        if (cur_mode == 0) a860_process_h(); else a860_process_g();
        return;
    }
    for (int t = maxt; t >= 0; t--) {
        int k = t / 2 + 1;
        if (k > rem) continue;
        cur_k[cur_nc] = k;
        cur_p[cur_nc] = t & 1;
        cur_nc++;
        a860_rec(rem - k, t);
        cur_nc--;
    }
}

#define A860_SUM(T, NAME)                                                        \
static T NAME(idx_t n) {                                                         \
    T bn = 1;                                                                    \
    for (idx_t i = 1; i <= n; i++) bn = bn * (T)(2 * i);                         \
    T s = 0;                                                                     \
    for (int c = 0; c < g_cnt; c++) {                                            \
        const int *d = g_data + (size_t)c * g_stride;                            \
        int nc = d[0];                                                           \
        T z = 1;                                                                 \
        int run = 0;                                                             \
        for (int i = 0; i < nc; i++) {                                           \
            int k = d[1 + 2 * i], p = d[2 + 2 * i];                              \
            if (i > 0 && k == d[2 * i - 1] && p == d[2 * i]) run++; else run = 1; \
            z = z * (T)(2 * k) * (T)run;                                         \
        }                                                                        \
        T cls = bn / z;                                                          \
        for (int j = 0; j < h_cnt; j++) {                                        \
            long long e = g_exp[(size_t)c * h_cnt + j];                          \
            if (e < 0) continue;                                                 \
            T one = 1;                                                           \
            s += cls * (T)h_size[j] * (one << (idx_t)e);                         \
        }                                                                        \
    }                                                                            \
    return s / (bn * (T)48);                                                     \
}

A860_SUM(seq_t, a860_sum_seq)
A860_SUM(unsigned __int128, a860_sum_u128)

static int a860_fixed(void) {
    seq_t x = 1;
    for (int i = 0; i < 200; i++) x = x + x;
    return x == 0;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    int fixed = a860_fixed();
    if (fixed && n >= 6) return 0; /* a(n) > 2^128 for n >= 6: not representable in any fixed width */

    h_cnt = 0;
    cur_nc = 0;
    cur_mode = 0;
    a860_rec(3, 5);

    g_cnt = 0;
    g_cap = 0;
    g_data = NULL;
    g_exp = NULL;
    g_stride = 1 + 2 * (int)n;
    cur_nc = 0;
    cur_mode = 1;
    a860_rec((int)n, 2 * (int)n - 1);

    seq_t res;
    if (fixed) res = a677_from_u128(a860_sum_u128(n));
    else res = a860_sum_seq(n);

    free(g_data);
    free(g_exp);
    g_data = NULL;
    g_exp = NULL;
    return res;
}
