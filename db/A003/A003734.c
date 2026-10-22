/*
 * A003734  Number of spanning trees with degrees 1 and 3 in C_5 X P_2n.
 * https://oeis.org/A003734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = b(2n), where b(m) = number of spanning trees of C_5 X P_m
 *            with all degrees 1 or 3. b(m) is computed by a transfer matrix
 *            over column states (connectivity partition of the 5 frontier
 *            vertices + which of them still need an edge to the next column).
 *            a(n) satisfies a linear recurrence a(n) = S...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   alloc_zero, grow_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"
#include "../utils/release.h"

#define NV 5
#define MAXS (52 * 32)
#define PMOD 2305843009213693951ULL
#define KTERMS 400

static int nst;
static int st_lab[MAXS][NV];
static int st_mask[MAXS];
static int code2st[3125 * 32];
static int init_cnt[MAXS];
static int *tr_from, *tr_to, *tr_mul;
static int ntr, captr;
static int acc[MAXS], touched[MAXS];
static bool built;
static int rec_len;
static int64_t *rec_c;
static seq_t *memo;
static idx_t memolen, memocap;

static int uf_find(int *par, int x) {
    while (par[x] != x) { par[x] = par[par[x]]; x = par[x]; }
    return x;
}

static int get_state(const int *lab, int mask) {
    int map[16], nl[NV], i, next = 0, code = 0, pw = 1;
    for (i = 0; i < 16; i++) map[i] = -1;
    for (i = 0; i < NV; i++) {
        if (map[lab[i]] < 0) map[lab[i]] = next++;
        nl[i] = map[lab[i]];
        code += nl[i] * pw;
        pw *= 5;
    }
    code = code * 32 + mask;
    if (code2st[code]) return code2st[code] - 1;
    for (i = 0; i < NV; i++) st_lab[nst][i] = nl[i];
    st_mask[nst] = mask;
    code2st[code] = ++nst;
    return nst - 1;
}

static void add_tr(int f, int t, int m) {
    if (ntr == captr) {
        captr = captr ? 2 * captr : 1024;
        tr_from = (int *)realloc(tr_from, sizeof(int) * captr);
        tr_to = (int *)realloc(tr_to, sizeof(int) * captr);
        tr_mul = (int *)realloc(tr_mul, sizeof(int) * captr);
    }
    tr_from[ntr] = f; tr_to[ntr] = t; tr_mul[ntr] = m; ntr++;
}

static bool is_final(int s) {
    int i;
    if (st_mask[s] != 0) return false;
    for (i = 0; i < NV; i++) if (st_lab[s][i] != 0) return false;
    return true;
}

static void build_states(void) {
    int E, i, j, k, s;
    int par[2 * NV], deg[NV], lab[NV], nl[NV];
    for (E = 0; E < 32; E++) {
        bool ok = true;
        int mask = 0;
        for (k = 0; k < 2 * NV; k++) par[k] = k;
        for (i = 0; i < NV; i++) deg[i] = 0;
        for (i = 0; i < NV && ok; i++) if ((E >> i) & 1) {
            int u = i, v = (i + 1) % NV;
            int ru = uf_find(par, u), rv = uf_find(par, v);
            if (ru == rv) { ok = false; break; }
            par[ru] = rv; deg[u]++; deg[v]++;
        }
        if (!ok) continue;
        for (i = 0; i < NV; i++) {
            if (deg[i] % 2 == 0) mask |= 1 << i;
            lab[i] = uf_find(par, i);
        }
        s = get_state(lab, mask);
        init_cnt[s]++;
    }
    for (s = 0; s < nst; s++) {
        int H = st_mask[s], nt = 0;
        bool alive = true;
        for (i = 0; i < NV; i++) lab[i] = st_lab[s][i];
        for (i = 0; i < NV && alive; i++) {
            bool hit = false;
            for (j = 0; j < NV; j++)
                if (lab[j] == lab[i] && ((H >> j) & 1)) hit = true;
            if (!hit) alive = false;
        }
        if (!alive) continue;
        for (i = 0; i < NV; i++) nl[i] = ((H >> i) & 1) ? lab[i] : NV + i;
        for (E = 0; E < 32; E++) {
            bool ok = true;
            int mask = 0, t, lab2[NV];
            for (k = 0; k < 2 * NV; k++) par[k] = k;
            for (i = 0; i < NV; i++) deg[i] = (H >> i) & 1;
            for (i = 0; i < NV; i++) if ((E >> i) & 1) {
                int u = i, v = (i + 1) % NV;
                int ru = uf_find(par, nl[u]), rv = uf_find(par, nl[v]);
                if (ru == rv) { ok = false; break; }
                par[ru] = rv; deg[u]++; deg[v]++;
            }
            if (!ok) continue;
            for (i = 0; i < NV; i++) {
                if (deg[i] % 2 == 0) mask |= 1 << i;
                lab2[i] = uf_find(par, nl[i]);
            }
            t = get_state(lab2, mask);
            if (!acc[t]) touched[nt++] = t;
            acc[t]++;
        }
        for (i = 0; i < nt; i++) {
            add_tr(s, touched[i], acc[touched[i]]);
            acc[touched[i]] = 0;
        }
    }
}

static uint64_t mulm(uint64_t x, uint64_t y) {
    return (uint64_t)(((unsigned __int128)x * y) % PMOD);
}

static uint64_t powm(uint64_t b, uint64_t e) {
    uint64_t r = 1;
    while (e) { if (e & 1) r = mulm(r, b); b = mulm(b, b); e >>= 1; }
    return r;
}

static int berlekamp_massey(const uint64_t *s, int n, uint64_t *C) {
    uint64_t *B = (uint64_t *)malloc(sizeof(uint64_t) * (n + 1));
    uint64_t *T = (uint64_t *)malloc(sizeof(uint64_t) * (n + 1));
    uint64_t b = 1;
    int L = 0, m = 1, i, k;
    for (i = 0; i <= n; i++) C[i] = B[i] = 0;
    C[0] = B[0] = 1;
    for (k = 0; k < n; k++) {
        uint64_t d = s[k], coef;
        for (i = 1; i <= L; i++) d = (d + mulm(C[i], s[k - i])) % PMOD;
        if (d == 0) { m++; continue; }
        coef = mulm(d, powm(b, PMOD - 2));
        if (2 * L <= k) {
            memcpy(T, C, sizeof(uint64_t) * (n + 1));
            for (i = m; i <= n; i++) C[i] = (C[i] + PMOD - mulm(coef, B[i - m])) % PMOD;
            L = k + 1 - L;
            memcpy(B, T, sizeof(uint64_t) * (n + 1));
            b = d; m = 1;
        } else {
            for (i = m; i <= n; i++) C[i] = (C[i] + PMOD - mulm(coef, B[i - m])) % PMOD;
            m++;
        }
    }
    free(B); free(T);
    return L;
}

static void build_all(void) {
    uint64_t *v, *w, *sq, *C;
    int i, m, t;
    seq_t *ev, *ew;
    build_states();

    /* terms modulo PMOD */
    v = (uint64_t *)malloc(sizeof(uint64_t) * nst);
    w = (uint64_t *)malloc(sizeof(uint64_t) * nst);
    sq = (uint64_t *)malloc(sizeof(uint64_t) * KTERMS);
    C = (uint64_t *)malloc(sizeof(uint64_t) * (KTERMS + 1));
    for (i = 0; i < nst; i++) v[i] = (uint64_t)init_cnt[i];
    for (m = 1; m <= 2 * KTERMS; m++) {
        if (m % 2 == 0) {
            uint64_t sum = 0;
            for (i = 0; i < nst; i++) if (is_final(i)) sum = (sum + v[i]) % PMOD;
            sq[m / 2 - 1] = sum;
        }
        if (m == 2 * KTERMS) break;
        for (i = 0; i < nst; i++) w[i] = 0;
        for (t = 0; t < ntr; t++)
            w[tr_to[t]] = (w[tr_to[t]] + mulm(v[tr_from[t]], (uint64_t)tr_mul[t])) % PMOD;
        for (i = 0; i < nst; i++) v[i] = w[i];
    }
    rec_len = berlekamp_massey(sq, KTERMS, C);
    rec_c = (int64_t *)malloc(sizeof(int64_t) * (rec_len + 1));
    for (i = 1; i <= rec_len; i++) {
        uint64_t c = (PMOD - C[i]) % PMOD;
        rec_c[i] = (c > PMOD / 2) ? -(int64_t)(PMOD - c) : (int64_t)c;
    }
    free(v); free(w); free(sq); free(C);

    /* exact initial terms */
    memocap = (idx_t)(rec_len + 16);
    memo = alloc_zero(memocap);
    ev = alloc_zero((idx_t)nst);
    ew = alloc_zero((idx_t)nst);
    for (i = 0; i < nst; i++) ev[i] = (seq_t)init_cnt[i];
    for (m = 1; m <= 2 * rec_len; m++) {
        if (m % 2 == 0) {
            seq_t sum = 0;
            for (i = 0; i < nst; i++) if (is_final(i)) sum += ev[i];
            memo[m / 2 - 1] = sum;
        }
        if (m == 2 * rec_len) break;
        for (i = 0; i < nst; i++) ew[i] = 0;
        for (t = 0; t < ntr; t++)
            ew[tr_to[t]] += ev[tr_from[t]] * (seq_t)tr_mul[t];
        for (i = 0; i < nst; i++) ev[i] = ew[i];
    }
    release(ev, (idx_t)nst);
    release(ew, (idx_t)nst);
    memolen = (idx_t)rec_len;
    built = true;
}

seq_t a(idx_t n) {
    idx_t k;
    if (n < 1) return 0;
    if (!built) build_all();
    k = n - 1;
    if (k >= memocap) {
        idx_t nc = memocap;
        while (nc <= k) nc *= 2;
        memo = grow_zero(memo, memocap, nc);
        memocap = nc;
    }
    while (memolen <= k) {
        seq_t pos = 0, neg = 0;
        int i;
        for (i = 1; i <= rec_len; i++) {
            int64_t c = rec_c[i];
            if (c > 0) pos += memo[memolen - i] * (seq_t)(uint64_t)c;
            else if (c < 0) neg += memo[memolen - i] * (seq_t)(uint64_t)(-c);
        }
        memo[memolen] = pos - neg;
        memolen++;
    }
    return memo[k];
}
