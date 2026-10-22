/*
 * A003732  Number of Hamiltonian paths in C_5 X P_n.
 * https://oeis.org/A003732
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   transfer-matrix count of Hamiltonian paths in C_5 X P_n. The
 *            5-cycle columns are processed one at a time, and each frontier
 *            state records how the paths connect. Berlekamp-Massey mod p
 *            (2^61-1) on the transfer-matrix terms finds the linear
 *            recurrence (12 terms per Faase): a(n) = sum_i c_i*a(n-i)....
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   alloc_zero, grow_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"
#include "../utils/release.h"

#define NCODE 32768
#define MODP 2305843009213693951ULL

static int st_idx[NCODE];
static int st_code[NCODE];
static int nst = 0;
static int *tr_from = NULL, *tr_to = NULL, *tr_mult = NULL;
static int ntr = 0, captr = 0;
static int fin_cnt[NCODE];
static int acc_t[NCODE];
static int touch_l[NCODE];
static long long *rec_c = NULL;
static int rec_L = 0;
static seq_t *memo = NULL;
static idx_t memo_cap = 0, memo_filled = 0;
static int ready = 0;

static int uf_find(int *p, int x) {
    while (p[x] != x) { p[x] = p[p[x]]; x = p[x]; }
    return x;
}

static int uf_unite(int *p, int x, int y) {
    x = uf_find(p, x); y = uf_find(p, y);
    if (x == y) return 0;
    p[x] = y;
    return 1;
}

static int step(const int *f, int C, int R, int fin, int *outcode) {
    int deg, p[12], cnt[8], has[12], map[12];
    int j, l, eold = 0, enew = 0, nl = 0, code = 0, r;
    memset(cnt, 0, sizeof cnt);
    for (j = 0; j < 5; j++) if (f[j]) cnt[f[j]]++;
    for (l = 1; l < 8; l++) if (cnt[l] == 1) eold++;
    for (j = 0; j < 5; j++) {
        deg = (f[j] != 0) + ((C >> j) & 1) + ((C >> ((j + 4) % 5)) & 1) + ((R >> j) & 1);
        if (deg < 1 || deg > 2) return 0;
        if (deg == 1) enew++;
    }
    if (eold + enew > 2) return 0;
    for (j = 0; j < 12; j++) p[j] = j;
    for (j = 0; j < 5; j++)
        if (f[j] && !uf_unite(p, f[j] - 1, 7 + j)) return 0;
    for (j = 0; j < 5; j++)
        if (((C >> j) & 1) && !uf_unite(p, 7 + j, 7 + (j + 1) % 5)) return 0;
    if (fin) {
        r = uf_find(p, 7);
        for (j = 1; j < 5; j++) if (uf_find(p, 7 + j) != r) return 0;
        *outcode = 0;
        return 1;
    }
    memset(has, 0, sizeof has);
    memset(map, 0, sizeof map);
    for (j = 0; j < 5; j++) if ((R >> j) & 1) has[uf_find(p, 7 + j)] = 1;
    for (j = 0; j < 5; j++) if (!has[uf_find(p, 7 + j)]) return 0;
    for (j = 0; j < 5; j++) {
        if ((R >> j) & 1) {
            r = uf_find(p, 7 + j);
            if (!map[r]) map[r] = ++nl;
            code |= map[r] << (3 * j);
        }
    }
    *outcode = code;
    return 1;
}

static int add_state(int code) {
    if (st_idx[code] < 0) { st_code[nst] = code; st_idx[code] = nst++; }
    return st_idx[code];
}

static void push_tr(int s, int t, int m) {
    if (ntr == captr) {
        captr = captr ? 2 * captr : 1024;
        tr_from = (int *)realloc(tr_from, sizeof(int) * captr);
        tr_to = (int *)realloc(tr_to, sizeof(int) * captr);
        tr_mult = (int *)realloc(tr_mult, sizeof(int) * captr);
    }
    tr_from[ntr] = s; tr_to[ntr] = t; tr_mult[ntr] = m; ntr++;
}

static uint64_t mulmod(uint64_t x, uint64_t y) {
    return (uint64_t)(((unsigned __int128)x * y) % MODP);
}

static uint64_t powmod(uint64_t b, uint64_t e) {
    uint64_t r = 1;
    while (e) { if (e & 1) r = mulmod(r, b); b = mulmod(b, b); e >>= 1; }
    return r;
}

static void init(void) {
    int s, C, R, nc, t, i, k, j, f[5], ntouch, K;
    uint64_t *v, *w, *sp, *Cp, *Bp, *Tp, *tmp;
    memset(st_idx, -1, sizeof st_idx);
    memset(acc_t, 0, sizeof acc_t);
    add_state(0);
    for (s = 0; s < nst; s++) {
        for (j = 0; j < 5; j++) f[j] = (st_code[s] >> (3 * j)) & 7;
        fin_cnt[s] = 0;
        for (C = 0; C < 32; C++)
            if (step(f, C, 0, 1, &nc)) fin_cnt[s]++;
        ntouch = 0;
        for (C = 0; C < 32; C++)
            for (R = 1; R < 32; R++)
                if (step(f, C, R, 0, &nc)) {
                    t = add_state(nc);
                    if (acc_t[t] == 0) touch_l[ntouch++] = t;
                    acc_t[t]++;
                }
        for (i = 0; i < ntouch; i++) {
            push_tr(s, touch_l[i], acc_t[touch_l[i]]);
            acc_t[touch_l[i]] = 0;
        }
    }

    /* terms mod p */
    K = 2 * nst + 20;
    v = (uint64_t *)calloc(nst, sizeof(uint64_t));
    w = (uint64_t *)calloc(nst, sizeof(uint64_t));
    sp = (uint64_t *)calloc(K, sizeof(uint64_t));
    v[0] = 1;
    for (k = 0; k < K; k++) {
        uint64_t acc = 0;
        for (i = 0; i < nst; i++)
            if (fin_cnt[i]) acc = (acc + mulmod((uint64_t)fin_cnt[i], v[i])) % MODP;
        sp[k] = acc;
        memset(w, 0, sizeof(uint64_t) * nst);
        for (i = 0; i < ntr; i++)
            w[tr_to[i]] = (w[tr_to[i]] + mulmod((uint64_t)tr_mult[i], v[tr_from[i]])) % MODP;
        tmp = v; v = w; w = tmp;
    }

    /* Berlekamp-Massey mod p */
    Cp = (uint64_t *)calloc(K + 1, sizeof(uint64_t));
    Bp = (uint64_t *)calloc(K + 1, sizeof(uint64_t));
    Tp = (uint64_t *)calloc(K + 1, sizeof(uint64_t));
    {
        int L = 0, m = 1;
        uint64_t b = 1;
        Cp[0] = 1; Bp[0] = 1;
        for (k = 0; k < K; k++) {
            uint64_t d = sp[k], coef;
            for (i = 1; i <= L; i++) d = (d + mulmod(Cp[i], sp[k - i])) % MODP;
            if (d == 0) { m++; continue; }
            coef = mulmod(d, powmod(b, MODP - 2));
            if (2 * L <= k) {
                memcpy(Tp, Cp, sizeof(uint64_t) * (K + 1));
                for (i = 0; i + m <= K; i++)
                    Cp[i + m] = (Cp[i + m] + MODP - mulmod(coef, Bp[i])) % MODP;
                L = k + 1 - L;
                memcpy(Bp, Tp, sizeof(uint64_t) * (K + 1));
                b = d; m = 1;
            } else {
                for (i = 0; i + m <= K; i++)
                    Cp[i + m] = (Cp[i + m] + MODP - mulmod(coef, Bp[i])) % MODP;
                m++;
            }
        }
        rec_L = L;
        rec_c = (long long *)calloc(L + 1, sizeof(long long));
        for (i = 1; i <= L; i++) {
            uint64_t c = (MODP - Cp[i]) % MODP;
            rec_c[i] = (c > MODP / 2) ? -(long long)(MODP - c) : (long long)c;
        }
    }
    free(Cp); free(Bp); free(Tp); free(sp); free(v); free(w);

    /* exact initial terms s_0..s_{L-1} by the transfer matrix */
    {
        seq_t *ev = alloc_zero((idx_t)nst);
        seq_t *ew = alloc_zero((idx_t)nst);
        seq_t *et;
        idx_t cap = (idx_t)(rec_L < 16 ? 16 : rec_L);
        memo = alloc_zero(cap);
        memo_cap = cap;
        ev[0] = 1;
        for (k = 0; k < rec_L; k++) {
            seq_t acc = 0;
            for (i = 0; i < nst; i++)
                if (fin_cnt[i]) acc += ev[i] * (idx_t)fin_cnt[i];
            memo[k] = acc;
            for (i = 0; i < nst; i++) ew[i] = 0;
            for (i = 0; i < ntr; i++)
                ew[tr_to[i]] += ev[tr_from[i]] * (idx_t)tr_mult[i];
            et = ev; ev = ew; ew = et;
        }
        memo_filled = (idx_t)rec_L;
        release(ev, (idx_t)nst);
        release(ew, (idx_t)nst);
    }
    ready = 1;
}

static void extend(idx_t need) {
    idx_t k;
    int i;
    if (need > memo_cap) {
        idx_t nc = memo_cap * 2;
        if (nc < need) nc = need;
        memo = grow_zero(memo, memo_cap, nc);
        memo_cap = nc;
    }
    for (k = memo_filled; k < need; k++) {
        seq_t pos = 0, neg = 0;
        for (i = 1; i <= rec_L; i++) {
            long long c = rec_c[i];
            if (c > 0) pos += memo[k - i] * (idx_t)c;
            else if (c < 0) neg += memo[k - i] * (idx_t)(-c);
        }
        memo[k] = pos - neg;
    }
    memo_filled = need;
}

seq_t a(idx_t n) {
    idx_t k;
    if (n < 1) return 0;
    if (!ready) init();
    k = n - 1;
    if (k >= memo_filled) extend(k + 1);
    return memo[k];
}
