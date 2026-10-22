/*
 * A002835  Solid partitions of n which are restricted to two planes.
 * https://oeis.org/A002835
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   search, not a closed form. a(n) counts pairs of plane partitions
 *            P >= Q (componentwise) with |P| + |Q| = n, i.e. 3-D arrays with
 *            entries in {0,1,2} that are nonincreasing in every direction.
 *            Built row by row: each row is a pair of nested nonincreasing
 *            sequences and must be <= the row before it. T...
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   key_hash
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/key_hash.h"

typedef unsigned __int128 u128;

static uint16_t *pool = NULL;
static size_t pool_len = 0, pool_cap = 0;
static uint32_t *h_off = NULL;
static u128 *h_val = NULL;
static size_t h_cap = 0, h_cnt = 0;

static size_t find_slot(const uint16_t *key, size_t klen, uint64_t hv) {
    size_t mask = h_cap - 1, i = (size_t)(hv & mask);
    while (h_off[i]) {
        size_t o = h_off[i] - 1;
        if (2 + 2 * (size_t)pool[o + 1] == klen &&
            memcmp(pool + o, key, klen * sizeof(uint16_t)) == 0)
            return i;
        i = (i + 1) & mask;
    }
    return i;
}

static void table_init(void) {
    if (h_cap) return;
    h_cap = (size_t)1 << 16;
    h_off = (uint32_t *)calloc(h_cap, sizeof(uint32_t));
    h_val = (u128 *)malloc(h_cap * sizeof(u128));
}

static void table_grow(void) {
    size_t oc = h_cap, i;
    uint32_t *oo = h_off;
    u128 *ov = h_val;
    h_cap *= 2;
    h_off = (uint32_t *)calloc(h_cap, sizeof(uint32_t));
    h_val = (u128 *)malloc(h_cap * sizeof(u128));
    for (i = 0; i < oc; i++) {
        if (oo[i]) {
            size_t o = oo[i] - 1;
            size_t klen = 2 + 2 * (size_t)pool[o + 1];
            size_t s = find_slot(pool + o, klen, key_hash(pool + o, klen));
            h_off[s] = oo[i];
            h_val[s] = ov[i];
        }
    }
    free(oo);
    free(ov);
}

static u128 f_count(const uint16_t *key);

typedef struct {
    const uint16_t *rA, *rB;
    int L, m;
    uint16_t *sA, *sB, *kb, *tb;
    u128 sum;
} Gen;

static void gen(Gen *g, int j, int pa, int pb, int w) {
    if (j > 0) {
        int mm = g->m - w, t, nl = 0;
        g->kb[0] = (uint16_t)mm;
        for (t = 0; t < j; t++) {
            int amax = mm / (t + 1), bmax = mm / (2 * (t + 1));
            int A = g->sA[t] < amax ? g->sA[t] : amax;
            int B;
            if (A == 0) break;
            B = g->sB[t];
            if (B > bmax) B = bmax;
            if (B > A) B = A;
            g->kb[2 + t] = (uint16_t)A;
            g->tb[t] = (uint16_t)B;
            nl = t + 1;
        }
        g->kb[1] = (uint16_t)nl;
        for (t = 0; t < nl; t++) g->kb[2 + nl + t] = g->tb[t];
        g->sum += f_count(g->kb);
    }
    if (j < g->L) {
        int amax = g->rA[j], av, bv;
        if (amax > pa) amax = pa;
        if (amax > g->m - w) amax = g->m - w;
        for (av = 1; av <= amax; av++) {
            int bmax = g->rB[j];
            if (bmax > pb) bmax = pb;
            if (bmax > av) bmax = av;
            if (bmax > g->m - w - av) bmax = g->m - w - av;
            for (bv = 0; bv <= bmax; bv++) {
                g->sA[j] = (uint16_t)av;
                g->sB[j] = (uint16_t)bv;
                gen(g, j + 1, av, bv, w + av + bv);
            }
        }
    }
}

static u128 f_count(const uint16_t *key) {
    int m = key[0], L = key[1];
    size_t klen, slot;
    uint64_t hv;
    Gen g;
    if (m == 0) return 1;
    if (L == 0) return 0;
    klen = 2 + 2 * (size_t)L;
    hv = key_hash(key, klen);
    slot = find_slot(key, klen, hv);
    if (h_off[slot]) return h_val[slot];

    g.rA = key + 2;
    g.rB = key + 2 + L;
    g.L = L;
    g.m = m;
    g.sA = (uint16_t *)malloc((size_t)L * sizeof(uint16_t));
    g.sB = (uint16_t *)malloc((size_t)L * sizeof(uint16_t));
    g.tb = (uint16_t *)malloc((size_t)L * sizeof(uint16_t));
    g.kb = (uint16_t *)malloc((2 + 2 * (size_t)L) * sizeof(uint16_t));
    g.sum = 0;
    gen(&g, 0, 0xFFFF, 0xFFFF, 0);
    free(g.sA);
    free(g.sB);
    free(g.tb);
    free(g.kb);

    if ((h_cnt + 1) * 2 > h_cap) table_grow();
    if (pool_len + klen > pool_cap) {
        size_t nc = pool_cap ? pool_cap : 4096;
        while (nc < pool_len + klen) nc *= 2;
        pool = (uint16_t *)realloc(pool, nc * sizeof(uint16_t));
        pool_cap = nc;
    }
    slot = find_slot(key, klen, hv);
    memcpy(pool + pool_len, key, klen * sizeof(uint16_t));
    h_off[slot] = (uint32_t)(pool_len + 1);
    h_val[slot] = g.sum;
    pool_len += klen;
    h_cnt++;
    return g.sum;
}

static seq_t to_seq(u128 v) {
    seq_t r = 0;
    int i;
    for (i = 120; i >= 0; i -= 8) {
        r = (seq_t)(r << 8);
        r = (seq_t)(r + (int)((v >> i) & 0xFF));
    }
    return r;
}

seq_t a(idx_t n) {
    int N = (int)n, L = 0, j;
    uint16_t *key;
    u128 v;
    if (n == 0) return 1;
    table_init();
    key = (uint16_t *)malloc((2 + 2 * (size_t)N) * sizeof(uint16_t));
    for (j = 0; j < N; j++) {
        int A = N / (j + 1);
        if (A == 0) break;
        L = j + 1;
    }
    key[0] = (uint16_t)N;
    key[1] = (uint16_t)L;
    for (j = 0; j < L; j++) {
        int A = N / (j + 1), B = N / (2 * (j + 1));
        if (B > A) B = A;
        key[2 + j] = (uint16_t)A;
        key[2 + L + j] = (uint16_t)B;
    }
    v = f_count(key);
    free(key);
    return to_seq(v);
}
