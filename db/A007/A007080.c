/*
 * A007080  Number of labeled Eulerian digraphs with n nodes.
 * https://oeis.org/A007080
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = [x_1^(n-1)*...*x_n^(n-1)] Product_{1<=i<j<=n} (x_i+x_j)^2
 *            = Sum_{s} T(s)^2, summed over tournament score vectors s, where
 *            T(s) is the number of labeled tournaments with score vector s.
 *            The coefficient is extracted by removing one vertex at a time,
 *            using a DP over multisets of remaining exp...
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   alloc_zero, lv_hash, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/lv_hash.h"

typedef struct {
    idx_t k;
    size_t cnt, cap;
    unsigned char *keys;
    seq_t *w;
    size_t *ht;
    size_t hmask;
} Level;

static void lv_init(Level *L, idx_t k) {
    L->k = k;
    L->cnt = 0;
    L->cap = 64;
    L->keys = (unsigned char *)malloc(L->cap * (k ? k : 1));
    L->w = alloc_zero(L->cap);
    L->hmask = 127;
    L->ht = (size_t *)calloc(L->hmask + 1, sizeof(size_t));
}

static void lv_free(Level *L) {
    free(L->keys);
    release(L->w, L->cap);
    free(L->ht);
}

static void lv_rehash(Level *L) {
    size_t nm = (L->hmask + 1) * 2 - 1, i;
    free(L->ht);
    L->ht = (size_t *)calloc(nm + 1, sizeof(size_t));
    L->hmask = nm;
    for (i = 0; i < L->cnt; i++) {
        size_t p = (size_t)(lv_hash(L->keys + i * L->k, L->k) & nm);
        while (L->ht[p]) p = (p + 1) & nm;
        L->ht[p] = i + 1;
    }
}

static void lv_add(Level *L, const unsigned char *key, seq_t val) {
    idx_t k = L->k;
    size_t p = (size_t)(lv_hash(key, k) & L->hmask);
    while (L->ht[p]) {
        size_t i = L->ht[p] - 1;
        if (memcmp(L->keys + i * k, key, k) == 0) { L->w[i] += val; return; }
        p = (p + 1) & L->hmask;
    }
    if (L->cnt == L->cap) {
        size_t nc = L->cap * 2, i;
        seq_t *nw = alloc_zero(nc);
        for (i = 0; i < L->cnt; i++) nw[i] = L->w[i];
        release(L->w, L->cap);
        L->w = nw;
        L->keys = (unsigned char *)realloc(L->keys, nc * (k ? k : 1));
        L->cap = nc;
    }
    memcpy(L->keys + L->cnt * k, key, k);
    L->w[L->cnt] = val;
    L->cnt++;
    L->ht[p] = L->cnt;
    if (2 * L->cnt > L->hmask + 1) lv_rehash(L);
}

static Level *g_next;
static idx_t g_nc;
static idx_t g_u[256], g_m[256], g_suf[257];
static idx_t g_cnt[512];
static idx_t g_maxval;

static uint64_t small_binom(idx_t m, idx_t b) {
    uint64_t r = 1;
    idx_t i;
    if (b > m) return 0;
    if (b > m - b) b = m - b;
    for (i = 1; i <= b; i++) r = r * (uint64_t)(m - b + i) / (uint64_t)i;
    return r;
}

static void emit(seq_t w) {
    idx_t k = g_next->k, t = 0, v, j;
    unsigned char arr[256], comp[256];
    const unsigned char *key;
    long long s = 0;
    for (v = 0; v <= g_maxval; v++)
        for (j = 0; j < g_cnt[v]; j++) arr[t++] = (unsigned char)v;
    for (t = 0; t < k; t++) {
        s += arr[t];
        if (s < (long long)(t + 1) * (long long)t) return;
    }
    for (t = 0; t < k; t++)
        comp[t] = (unsigned char)(2 * (k - 1) - arr[k - 1 - t]);
    key = arr;
    if (comp[0] < arr[0]) key = comp;
    else if (comp[0] == arr[0] && memcmp(comp, arr, k) < 0) key = comp;
    lv_add(g_next, key, w);
}

static void dfs(idx_t c, idx_t B, seq_t w) {
    idx_t u, m, b1, b2, b0;
    if (c == g_nc) {
        if (B == 0) emit(w);
        return;
    }
    if (B > g_suf[c]) return;
    u = g_u[c];
    m = g_m[c];
    for (b2 = 0; 2 * b2 <= B && b2 <= m; b2++) {
        for (b1 = 0; b1 + 2 * b2 <= B && b1 + b2 <= m; b1++) {
            uint64_t cf;
            seq_t nw;
            b0 = m - b1 - b2;
            if (b0 && u < 2) continue;
            if (b1 && u < 1) continue;
            if (b0) g_cnt[u - 2] += b0;
            if (b1) g_cnt[u - 1] += b1;
            g_cnt[u] += b2;
            cf = small_binom(m, b2) * small_binom(m - b2, b1);
            nw = (w * (seq_t)cf) << b1;
            dfs(c + 1, B - b1 - 2 * b2, nw);
            if (b0) g_cnt[u - 2] -= b0;
            if (b1) g_cnt[u - 1] -= b1;
            g_cnt[u] -= b2;
        }
    }
}

seq_t a(idx_t n) {
    Level cur, nxt;
    unsigned char key[256];
    idx_t k, i, j;
    seq_t res;
    if (n == 1) return 1;
    if (n > 120) return 0;
    g_maxval = 2 * n;
    for (i = 0; i < 512; i++) g_cnt[i] = 0;
    lv_init(&cur, n);
    for (i = 0; i < n; i++) key[i] = (unsigned char)(n - 1);
    lv_add(&cur, key, (seq_t)1);
    for (k = n; k >= 2; k--) {
        size_t si;
        lv_init(&nxt, k - 1);
        g_next = &nxt;
        for (si = 0; si < cur.cnt; si++) {
            const unsigned char *r = cur.keys + si * k;
            idx_t v = r[0];
            g_nc = 0;
            for (j = 1; j < k; j++) {
                if (g_nc > 0 && g_u[g_nc - 1] == r[j]) g_m[g_nc - 1]++;
                else { g_u[g_nc] = r[j]; g_m[g_nc] = 1; g_nc++; }
            }
            g_suf[g_nc] = 0;
            for (j = g_nc; j > 0; j--) g_suf[j - 1] = g_suf[j] + 2 * g_m[j - 1];
            dfs(0, v, cur.w[si]);
        }
        lv_free(&cur);
        cur = nxt;
    }
    res = 0;
    if (cur.cnt) res = cur.w[0];
    lv_free(&cur);
    return res;
}
