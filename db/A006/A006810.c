/*
 * A006810  Bond percolation series for mean cluster size on directed cubic
 *          lattice.
 * https://oeis.org/A006810
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = [p^n] S(p), where S(p) = Sum_{t>=0} E|W_t| and W_t is the
 *            set of sites at level t (x+y+z = t) that the origin reaches on
 *            the directed cubic lattice, with each bond
 *            (x,y,z)->(x+1,y,z),(x,y+1,z),(x,y,z+1) open with probability p.
 *            Computed exactly by a level-by-level transfer matrix over the...
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int cnt, cap, hcap;
    uint16_t *keys;
    unsigned char *len;
    uint64_t *poly;
    int *ht;
} lvl_t;

static int g_N, g_D, g_maxw;
static lvl_t *g_next;
static uint16_t *g_cand, *g_chosen;
static int *g_candk;
static int g_ncand, g_budget;
static uint64_t *g_stack;
static uint64_t *g_ans = NULL;
static long long g_have = -1;

static void lvl_init(lvl_t *L) {
    L->cnt = 0; L->cap = 0; L->hcap = 0;
    L->keys = NULL; L->len = NULL; L->poly = NULL; L->ht = NULL;
}

static void lvl_free(lvl_t *L) {
    free(L->keys); free(L->len); free(L->poly); free(L->ht);
    lvl_init(L);
}

static uint64_t hkey(const uint16_t *k, int len) {
    uint64_t h = 1469598103934665603ULL;
    int i;
    for (i = 0; i < len; i++) { h ^= (uint64_t)k[i]; h *= 1099511628211ULL; }
    h ^= (uint64_t)len * 0x9E3779B97F4A7C15ULL;
    return h ^ (h >> 29);
}

static void lvl_rehash(lvl_t *L, int nh) {
    int i, m = nh - 1;
    free(L->ht);
    L->ht = (int *)malloc(sizeof(int) * (size_t)nh);
    for (i = 0; i < nh; i++) L->ht[i] = -1;
    L->hcap = nh;
    for (i = 0; i < L->cnt; i++) {
        uint64_t h = hkey(L->keys + (size_t)i * g_maxw, L->len[i]);
        int pos = (int)(h & (uint64_t)m);
        while (L->ht[pos] >= 0) pos = (pos + 1) & m;
        L->ht[pos] = i;
    }
}

static void lvl_add(lvl_t *L, const uint16_t *k, int len, const uint64_t *p) {
    uint64_t h;
    int m, pos, id, d;
    if (L->hcap == 0 || 2 * (L->cnt + 1) > L->hcap)
        lvl_rehash(L, L->hcap ? 2 * L->hcap : 1024);
    h = hkey(k, len);
    m = L->hcap - 1;
    pos = (int)(h & (uint64_t)m);
    while (L->ht[pos] >= 0) {
        id = L->ht[pos];
        if (L->len[id] == len &&
            memcmp(L->keys + (size_t)id * g_maxw, k, sizeof(uint16_t) * (size_t)len) == 0) {
            uint64_t *q = L->poly + (size_t)id * g_D;
            for (d = 0; d < g_D; d++) q[d] += p[d];
            return;
        }
        pos = (pos + 1) & m;
    }
    if (L->cnt == L->cap) {
        int nc = L->cap ? 2 * L->cap : 1024;
        L->keys = (uint16_t *)realloc(L->keys, sizeof(uint16_t) * (size_t)nc * g_maxw);
        L->len = (unsigned char *)realloc(L->len, (size_t)nc);
        L->poly = (uint64_t *)realloc(L->poly, sizeof(uint64_t) * (size_t)nc * g_D);
        L->cap = nc;
    }
    id = L->cnt++;
    memcpy(L->keys + (size_t)id * g_maxw, k, sizeof(uint16_t) * (size_t)len);
    L->len[id] = (unsigned char)len;
    memcpy(L->poly + (size_t)id * g_D, p, sizeof(uint64_t) * (size_t)g_D);
    L->ht[pos] = id;
}

static void rec(int i, int wet) {
    uint64_t *src = g_stack + (size_t)i * g_D;
    uint64_t *dst = src + g_D;
    int k, d, r;
    if (i == g_ncand) {
        if (wet > 0) lvl_add(g_next, g_chosen, wet, src);
        return;
    }
    k = g_candk[i];
    memcpy(dst, src, sizeof(uint64_t) * (size_t)g_D);
    for (r = 0; r < k; r++)
        for (d = g_D - 1; d >= 1; d--) dst[d] -= dst[d - 1];
    rec(i + 1, wet);                       /* site dry: (1-p)^k */
    if (wet + 1 <= g_budget) {             /* site wet: 1-(1-p)^k */
        for (d = 0; d < g_D; d++) dst[d] = src[d] - dst[d];
        g_chosen[wet] = g_cand[i];
        rec(i + 1, wet + 1);
    }
}

static void compute(int N) {
    lvl_t cur, nxt;
    uint16_t k0 = 0;
    uint64_t *p0;
    uint16_t *tmp;
    int t, s, d, i, j;

    g_N = N; g_D = N + 1; g_maxw = N + 1;
    free(g_ans);
    g_ans = (uint64_t *)calloc((size_t)g_D, sizeof(uint64_t));
    g_stack = (uint64_t *)malloc(sizeof(uint64_t) * (size_t)(3 * g_maxw + 2) * g_D);
    g_cand = (uint16_t *)malloc(sizeof(uint16_t) * (size_t)(3 * g_maxw + 2));
    tmp = (uint16_t *)malloc(sizeof(uint16_t) * (size_t)(3 * g_maxw + 2));
    g_candk = (int *)malloc(sizeof(int) * (size_t)(3 * g_maxw + 2));
    g_chosen = (uint16_t *)malloc(sizeof(uint16_t) * (size_t)(3 * g_maxw + 2));
    p0 = (uint64_t *)calloc((size_t)g_D, sizeof(uint64_t));
    p0[0] = 1;

    lvl_init(&cur);
    lvl_add(&cur, &k0, 1, p0);
    for (t = 0; t <= N; t++) {
        lvl_init(&nxt);
        g_next = &nxt;
        for (s = 0; s < cur.cnt; s++) {
            uint64_t *P = cur.poly + (size_t)s * g_D;
            uint16_t *K = cur.keys + (size_t)s * g_maxw;
            int len = cur.len[s], low = 0, nt = 0;
            while (low < g_D && P[low] == 0) low++;
            if (low > N) continue;
            for (d = 0; d < g_D; d++) g_ans[d] += (uint64_t)len * P[d];
            if (t == N) continue;
            for (i = 0; i < len; i++) {
                tmp[nt++] = K[i];              /* z+1 */
                tmp[nt++] = (uint16_t)(K[i] + 64); /* x+1 */
                tmp[nt++] = (uint16_t)(K[i] + 1);  /* y+1 */
            }
            for (i = 1; i < nt; i++) {
                uint16_t v = tmp[i];
                j = i - 1;
                while (j >= 0 && tmp[j] > v) { tmp[j + 1] = tmp[j]; j--; }
                tmp[j + 1] = v;
            }
            g_ncand = 0;
            for (i = 0; i < nt; i++) {
                if (g_ncand > 0 && g_cand[g_ncand - 1] == tmp[i]) g_candk[g_ncand - 1]++;
                else { g_cand[g_ncand] = tmp[i]; g_candk[g_ncand] = 1; g_ncand++; }
            }
            g_budget = N - low;
            memcpy(g_stack, P, sizeof(uint64_t) * (size_t)g_D);
            rec(0, 0);
        }
        lvl_free(&cur);
        cur = nxt;
    }
    lvl_free(&cur);
    free(p0); free(tmp); free(g_stack); free(g_cand); free(g_candk); free(g_chosen);
    g_have = N;
}

seq_t a(idx_t n) {
    if ((long long)n > g_have) compute((int)n);
    return (seq_t)g_ans[n];
}
