/*
 * A006576  Primitive repfigit numbers.
 * https://oeis.org/A006576
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   search, not a closed form. a(n) is the n-th Keith (repfigit)
 *            number whose digits have gcd 1. A Keith number with k >= 2
 *            digits d_0..d_{k-1} appears in the sequence that starts with its
 *            digits, where each later term is the sum of the previous k
 *            terms. Write term m as sum C[m][i]*d_i. For each k an...
 * Range:     u8 n <= 6, u16 n <= 19, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   kgcd, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/u128_to_seq.h"
#include "../utils/kgcd.h"

typedef unsigned __int128 ku128;
typedef __int128 ki128;

static ku128 *kl_list = NULL;
static size_t kl_cnt = 0, kl_cap = 0;
static int kl_next_k = 2;

static int ks_k;
static ki128 ks_e[64], ks_minr[65], ks_maxr[65];
static int ks_d[64];
static ku128 ks_p10[64];

static void kl_push(ku128 v) {
    if (kl_cnt == kl_cap) {
        kl_cap = kl_cap ? kl_cap * 2 : 64;
        kl_list = (ku128 *)realloc(kl_list, kl_cap * sizeof(ku128));
    }
    kl_list[kl_cnt++] = v;
}

static void ks_found(void) {
    int g = 0, i;
    ku128 v = 0;
    for (i = 0; i < ks_k; i++) g = kgcd(g, ks_d[i]);
    if (g != 1) return;
    for (i = 0; i < ks_k; i++) v = v * 10 + (ku128)ks_d[i];
    kl_push(v);
}

static void ks_dfs(int i, ki128 s) {
    int lo = (i == 0) ? 1 : 0, d;
    if (i == ks_k - 1) {
        ki128 e = ks_e[i];
        if (e == 0) {
            if (s != 0) return;
            for (d = lo; d <= 9; d++) { ks_d[i] = d; ks_found(); }
        } else {
            ki128 ns = -s, q;
            if (ns % e != 0) return;
            q = ns / e;
            if (q < lo || q > 9) return;
            ks_d[i] = (int)q;
            ks_found();
        }
        return;
    }
    for (d = lo; d <= 9; d++) {
        ki128 t = s + ks_e[i] * d;
        if (t + ks_minr[i + 1] > 0) continue;
        if (t + ks_maxr[i + 1] < 0) continue;
        ks_d[i] = d;
        ks_dfs(i + 1, t);
    }
}

static int cmp_u128(const void *x, const void *y) {
    ku128 p = *(const ku128 *)x, q = *(const ku128 *)y;
    return (p < q) ? -1 : (p > q) ? 1 : 0;
}

static void keith_k(int k) {
    int i, j, head = 0;
    size_t start = kl_cnt, w, r;
    ku128 *win = (ku128 *)malloc((size_t)k * (size_t)k * sizeof(ku128));
    ku128 *row = (ku128 *)malloc((size_t)k * sizeof(ku128));
    ks_k = k;
    ks_p10[0] = 1;
    for (i = 1; i <= k; i++) ks_p10[i] = ks_p10[i - 1] * 10;
    memset(win, 0, (size_t)k * (size_t)k * sizeof(ku128));
    for (j = 0; j < k; j++) win[(size_t)j * k + j] = 1;
    for (;;) {
        ku128 hi = 0;
        for (i = 0; i < k; i++) {
            ku128 s = 0;
            for (j = 0; j < k; j++) s += win[(size_t)j * k + i];
            row[i] = s;
            hi += s;
        }
        hi *= 9;
        if (row[0] >= ks_p10[k]) break;
        if (hi >= ks_p10[k - 1]) {
            for (i = 0; i < k; i++) ks_e[i] = (ki128)row[i] - (ki128)ks_p10[k - 1 - i];
            ks_minr[k] = 0; ks_maxr[k] = 0;
            for (i = k - 1; i >= 1; i--) {
                ki128 v = ks_e[i] * 9;
                ks_minr[i] = ks_minr[i + 1] + (v < 0 ? v : 0);
                ks_maxr[i] = ks_maxr[i + 1] + (v > 0 ? v : 0);
            }
            ks_dfs(0, 0);
        }
        memcpy(win + (size_t)head * k, row, (size_t)k * sizeof(ku128));
        head = (head + 1) % k;
    }
    free(win);
    free(row);
    if (kl_cnt > start) {
        qsort(kl_list + start, kl_cnt - start, sizeof(ku128), cmp_u128);
        w = start + 1;
        for (r = start + 1; r < kl_cnt; r++)
            if (kl_list[r] != kl_list[w - 1]) kl_list[w++] = kl_list[r];
        kl_cnt = w;
    }
}

seq_t a(idx_t n) {
    while (kl_cnt < (size_t)n && kl_next_k < 38) {
        keith_k(kl_next_k);
        kl_next_k++;
    }
    return u128_to_seq(kl_list[n - 1]);
}
