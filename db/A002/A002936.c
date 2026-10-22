/*
 * A002936  Solid partitions of n, distinct along rows.
 * https://oeis.org/A002936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   no closed form; this is a memoized search. a(n) counts solid
 *            partitions of n (3-D arrays n(i,j,k) that are weakly decreasing
 *            in i, j and k) whose rows, read along k, are strictly
 *            decreasing. The array is split into layers along k: P_1, P_2,
 *            ... are nonempty plane partitions with P_{t+1} <= max(P_...
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   A_grow, hkey
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/A_grow.h"
#include "../utils/hkey.h"

static unsigned short **mk_keys = NULL;
static int *mk_lens = NULL;
static unsigned long long *mk_hash = NULL;
static seq_t *mk_vals = NULL;
static long mk_cnt = 0, mk_cap = 0;
static long *mh_slots = NULL;
static long mh_size = 0;

static long memo_lookup(const unsigned short *k, int len, unsigned long long h) {
    size_t s;
    if (mh_size == 0) return -1;
    s = (size_t)(h & (unsigned long long)(mh_size - 1));
    while (mh_slots[s] >= 0) {
        long id = mh_slots[s];
        if (mk_hash[id] == h && mk_lens[id] == len &&
            memcmp(mk_keys[id], k, (size_t)len * sizeof(unsigned short)) == 0)
            return id;
        s = (s + 1) & (size_t)(mh_size - 1);
    }
    return -1;
}

static void memo_insert(unsigned short *k, int len, unsigned long long h, seq_t v) {
    size_t s;
    long i;
    if ((mk_cnt + 1) * 2 > mh_size) {
        long ns = mh_size ? mh_size * 2 : 1024;
        long *sl = (long *)malloc(sizeof(long) * (size_t)ns);
        for (i = 0; i < ns; i++) sl[i] = -1;
        for (i = 0; i < mk_cnt; i++) {
            size_t t = (size_t)(mk_hash[i] & (unsigned long long)(ns - 1));
            while (sl[t] >= 0) t = (t + 1) & (size_t)(ns - 1);
            sl[t] = i;
        }
        free(mh_slots);
        mh_slots = sl;
        mh_size = ns;
    }
    if (mk_cnt == mk_cap) {
        long nc = mk_cap ? mk_cap * 2 : 1024;
        mk_keys = (unsigned short **)realloc(mk_keys, sizeof(unsigned short *) * (size_t)nc);
        mk_lens = (int *)realloc(mk_lens, sizeof(int) * (size_t)nc);
        mk_hash = (unsigned long long *)realloc(mk_hash, sizeof(unsigned long long) * (size_t)nc);
        mk_vals = A_grow(mk_vals, (idx_t)mk_cap, (idx_t)nc);
        mk_cap = nc;
    }
    mk_keys[mk_cnt] = k;
    mk_lens[mk_cnt] = len;
    mk_hash[mk_cnt] = h;
    mk_vals[mk_cnt] = v;
    s = (size_t)(h & (unsigned long long)(mh_size - 1));
    while (mh_slots[s] >= 0) s = (s + 1) & (size_t)(mh_size - 1);
    mh_slots[s] = mk_cnt;
    mk_cnt++;
}

typedef struct {
    int m;
    int bnr, bw;
    const int *blen;
    const int *bv;
    int *p;
    int *plen;
    seq_t total;
} Ctx;

static seq_t count_layers(int m, int nr, const int *len, const int *v, int w);

static void finish(Ctx *c, int nrows, int sum) {
    int m2, W, cnr, r, j;
    int *clen, *cv;
    if (sum == 0) return;
    m2 = c->m - sum;
    if (m2 == 0) { c->total += 1; return; }
    W = c->bw;
    clen = (int *)malloc(sizeof(int) * (size_t)(nrows + 1));
    cv = (int *)malloc(sizeof(int) * (size_t)nrows * (size_t)W);
    cnr = 0;
    for (r = 0; r < nrows; r++) {
        int L = 0;
        for (j = 0; j < c->plen[r]; j++) {
            int x = c->p[r * W + j] - 1;
            int lim = m2 / ((r + 1) * (j + 1));
            if (x > lim) x = lim;
            if (x <= 0) break;
            cv[r * W + j] = x;
            L++;
        }
        if (L == 0) break;
        clen[r] = L;
        cnr++;
    }
    if (cnr > 0) c->total += count_layers(m2, cnr, clen, cv, W);
    free(clen);
    free(cv);
}

static void gen(Ctx *c, int i, int j, int sum) {
    int W = c->bw;
    int cap = 0, v;
    if (i < c->bnr && j < c->blen[i]) {
        cap = c->bv[i * W + j];
        if (i > 0) {
            int up = (j < c->plen[i - 1]) ? c->p[(i - 1) * W + j] : 0;
            if (up < cap) cap = up;
        }
        if (j > 0 && c->p[i * W + j - 1] < cap) cap = c->p[i * W + j - 1];
        if (cap > c->m - sum) cap = c->m - sum;
    }
    /* end row i here */
    c->plen[i] = j;
    if (j == 0) finish(c, i, sum);
    else gen(c, i + 1, 0, sum);
    for (v = 1; v <= cap; v++) {
        c->p[i * W + j] = v;
        gen(c, i, j + 1, sum + v);
    }
}

static seq_t count_layers(int m, int nr, const int *len, const int *v, int w) {
    int klen = 1 + nr, r, j, pos;
    unsigned short *key;
    unsigned long long h;
    long id;
    Ctx c;
    seq_t res;
    for (r = 0; r < nr; r++) klen += len[r];
    key = (unsigned short *)malloc(sizeof(unsigned short) * (size_t)klen);
    pos = 0;
    key[pos++] = (unsigned short)m;
    for (r = 0; r < nr; r++) {
        for (j = 0; j < len[r]; j++) key[pos++] = (unsigned short)v[r * w + j];
        key[pos++] = 0;
    }
    h = hkey(key, klen);
    id = memo_lookup(key, klen, h);
    if (id >= 0) { free(key); return mk_vals[id]; }

    c.m = m;
    c.bnr = nr;
    c.bw = w;
    c.blen = len;
    c.bv = v;
    c.p = (int *)malloc(sizeof(int) * (size_t)nr * (size_t)w);
    c.plen = (int *)malloc(sizeof(int) * (size_t)(nr + 1));
    c.total = 0;
    gen(&c, 0, 0, 0);
    res = c.total;
    free(c.p);
    free(c.plen);
    memo_insert(key, klen, h, res);
    return res;
}

seq_t a(idx_t n) {
    int N = (int)n, i, j;
    int *len, *v;
    seq_t res;
    if (N <= 0) return 1;
    len = (int *)malloc(sizeof(int) * (size_t)N);
    v = (int *)malloc(sizeof(int) * (size_t)N * (size_t)N);
    for (i = 0; i < N; i++) {
        len[i] = N / (i + 1);
        for (j = 0; j < len[i]; j++) v[i * N + j] = N / ((i + 1) * (j + 1));
    }
    res = count_layers(N, N, len, v, N);
    free(len);
    free(v);
    return res;
}
