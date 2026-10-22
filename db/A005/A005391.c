/*
 * A005391  Number of Hamiltonian circuits on 2n X 8 rectangle.
 * https://oeis.org/A005391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   no closed form; a(n) is computed by a transfer-matrix count of
 *            Hamiltonian cycles on the 2n X 8 grid. The grid is swept one row
 *            at a time. A state records which of the 8 vertical edges cross
 *            the row boundary and how those edges are paired by the paths
 *            above
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 18,
 *            big any n
 * Helpers:   alloc_zero, grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"

#define W 8
#define MAXS 512
#define NKEYS 390625 /* 5^8 */

static int ns = 0;
static int *idx_of = NULL;
static int keys[MAXS];
static int tstart[MAXS + 1];
static int tto[MAXS * 128];
static int tcnt[MAXS * 128];
static int fcnt[MAXS];

static int uf_find(int *p, int x) {
    while (p[x] != x) { p[x] = p[p[x]]; x = p[x]; }
    return x;
}

static int uf_unite(int *p, int a, int b) {
    a = uf_find(p, a); b = uf_find(p, b);
    if (a == b) return 1;
    p[a] = b;
    return 0;
}

static int get_state(int key) {
    if (idx_of[key] < 0) {
        idx_of[key] = ns;
        keys[ns] = key;
        ns++;
    }
    return idx_of[key];
}

static void build(void) {
    int i, j, q, h;
    idx_of = (int *)malloc(NKEYS * sizeof(int));
    for (i = 0; i < NKEYS; i++) idx_of[i] = -1;
    get_state(0);
    tstart[0] = 0;
    for (q = 0; q < ns; q++) {
        int lab[W], k = keys[q], ln = 0;
        int lt[128], lc[128];
        for (i = 0; i < W; i++) { lab[i] = k % 5; k /= 5; }
        fcnt[q] = 0;
        for (h = 0; h < (1 << (W - 1)); h++) {
            int d[W], par[W], closures = 0, anyd = 0, ok = 1;
            for (i = 0; i < W; i++) {
                int u = lab[i] != 0;
                int hl = i > 0 ? (h >> (i - 1)) & 1 : 0;
                int hr = i < W - 1 ? (h >> i) & 1 : 0;
                d[i] = 2 - u - hl - hr;
                if (d[i] < 0 || d[i] > 1) { ok = 0; break; }
                if (d[i]) anyd = 1;
            }
            if (!ok) continue;
            for (i = 0; i < W; i++) par[i] = i;
            for (i = 0; i < W; i++)
                if (lab[i])
                    for (j = i + 1; j < W; j++)
                        if (lab[j] == lab[i]) closures += uf_unite(par, i, j);
            for (i = 0; i < W - 1; i++)
                if ((h >> i) & 1) closures += uf_unite(par, i, i + 1);
            if (!anyd) {
                if (closures == 1) fcnt[q]++;
                continue;
            }
            if (closures) continue;
            {
                int map[W], nl[W], next = 1, key = 0, pw = 1, t, m;
                for (i = 0; i < W; i++) map[i] = 0;
                for (i = 0; i < W; i++) {
                    nl[i] = 0;
                    if (d[i]) {
                        int r = uf_find(par, i);
                        if (!map[r]) map[r] = next++;
                        nl[i] = map[r];
                    }
                }
                for (i = 0; i < W; i++) { key += nl[i] * pw; pw *= 5; }
                t = get_state(key);
                for (m = 0; m < ln; m++) if (lt[m] == t) break;
                if (m == ln) { lt[ln] = t; lc[ln] = 0; ln++; }
                lc[m]++;
            }
        }
        for (i = 0; i < ln; i++) {
            tto[tstart[q] + i] = lt[i];
            tcnt[tstart[q] + i] = lc[i];
        }
        tstart[q + 1] = tstart[q] + ln;
    }
}

static seq_t *vec = NULL, *nvec = NULL;
static idx_t rows_done = 0;
static seq_t *ans = NULL;
static char *have = NULL;
static idx_t ans_len = 0;

static void reset_vec(void) {
    int s;
    for (s = 0; s < ns; s++) vec[s] = 0;
    vec[0] = 1;
    rows_done = 0;
}

static void step(void) {
    int s, k;
    seq_t *tmp;
    for (s = 0; s < ns; s++) nvec[s] = 0;
    for (s = 0; s < ns; s++) {
        if (vec[s] == 0) continue;
        for (k = tstart[s]; k < tstart[s + 1]; k++)
            nvec[tto[k]] += vec[s] * (seq_t)tcnt[k];
    }
    tmp = vec; vec = nvec; nvec = tmp;
    rows_done++;
}

static seq_t finish(void) {
    seq_t r = 0;
    int s;
    for (s = 0; s < ns; s++)
        if (fcnt[s]) r += vec[s] * (seq_t)fcnt[s];
    return r;
}

static void store(idx_t m, seq_t v) {
    if (m >= ans_len) {
        idx_t nl = ans_len ? ans_len : 16;
        while (nl <= m) nl *= 2;
        ans = grow_zero(ans, ans_len, nl);
        have = (char *)realloc(have, (size_t)nl);
        memset(have + ans_len, 0, (size_t)(nl - ans_len));
        ans_len = nl;
    }
    ans[m] = v;
    have[m] = 1;
}

seq_t a(idx_t n) {
    idx_t target;
    if (n < 1) return 0;
    if (!idx_of) {
        build();
        vec = alloc_zero((idx_t)ns);
        nvec = alloc_zero((idx_t)ns);
        reset_vec();
    }
    if (n < ans_len && have[n]) return ans[n];
    target = 2 * n - 1;
    if (rows_done > target) reset_vec();
    while (rows_done < target) {
        step();
        if (rows_done & 1) store((rows_done + 1) / 2, finish());
    }
    if (!(n < ans_len && have[n])) store(n, finish());
    return ans[n];
}
