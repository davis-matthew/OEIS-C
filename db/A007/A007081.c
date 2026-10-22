/*
 * A007081  Number of labeled Eulerian oriented graphs with n nodes.
 * https://oeis.org/A007081
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = constant term of Product_{1<=i<j<=n} (1 + x_i/x_j +
 *            x_j/x_i) (equivalent to the OEIS coefficient formula). It is
 *            computed by a DP that removes one vertex at a time. The state is
 *            the multiset of partial balances (out-degree minus in-degree) of
 *            the remaining vertices. A multiset and its nega...
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 8, u64 n <= 11, u128 n <= 15,
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

typedef struct {
    uint8_t *keys;
    seq_t *vals;
    long long cnt, cap;
    long long *slots;
    long long ns;
} tab_t;

static int K_, B_, L_, need_, t_;
static int *cv_, *ci_, *suf_;
static uint8_t *nc_, *kb_;
static seq_t *Cb_;
static int nb_;
static tab_t *dst_;

static uint64_t hkey(const uint8_t *k) {
    uint64_t h = 1469598103934665603ULL;
    int i;
    for (i = 0; i < K_; i++) { h ^= k[i]; h *= 1099511628211ULL; }
    return h ^ (h >> 29);
}

static void tab_init(tab_t *T) {
    long long i;
    T->cnt = 0; T->cap = 512; T->ns = 1024;
    T->keys = (uint8_t *)malloc((size_t)T->cap * (size_t)K_);
    T->vals = alloc_zero((idx_t)T->cap);
    T->slots = (long long *)malloc((size_t)T->ns * sizeof(long long));
    for (i = 0; i < T->ns; i++) T->slots[i] = -1;
}

static void tab_free(tab_t *T) {
    free(T->keys);
    release(T->vals, (idx_t)T->cap);
    free(T->slots);
}

static void tab_add(tab_t *T, const uint8_t *key, seq_t x) {
    uint64_t mask, h;
    long long i;
    if ((T->cnt + 1) * 2 > T->ns) {
        long long nns = T->ns * 2;
        free(T->slots);
        T->slots = (long long *)malloc((size_t)nns * sizeof(long long));
        for (i = 0; i < nns; i++) T->slots[i] = -1;
        T->ns = nns;
        mask = (uint64_t)nns - 1;
        for (i = 0; i < T->cnt; i++) {
            h = hkey(T->keys + (size_t)i * K_) & mask;
            while (T->slots[h] != -1) h = (h + 1) & mask;
            T->slots[h] = i;
        }
    }
    mask = (uint64_t)T->ns - 1;
    h = hkey(key) & mask;
    while (T->slots[h] != -1) {
        long long idx = T->slots[h];
        if (memcmp(T->keys + (size_t)idx * K_, key, (size_t)K_) == 0) {
            T->vals[idx] += x;
            return;
        }
        h = (h + 1) & mask;
    }
    if (T->cnt == T->cap) {
        long long nc = T->cap * 2;
        T->keys = (uint8_t *)realloc(T->keys, (size_t)nc * (size_t)K_);
        T->vals = grow_zero(T->vals, (idx_t)T->cap, (idx_t)nc);
        T->cap = nc;
    }
    memcpy(T->keys + (size_t)T->cnt * K_, key, (size_t)K_);
    T->vals[T->cnt] = x;
    T->slots[h] = T->cnt;
    T->cnt++;
}

static seq_t cb(int a, int b) { return Cb_[(size_t)a * (nb_ + 1) + b]; }

static void emit(seq_t w) {
    int i, rev = 0;
    for (i = 0; i < K_; i++) {
        uint8_t x = nc_[i], y = nc_[K_ - 1 - i];
        if (x != y) { rev = (y < x); break; }
    }
    if (rev) for (i = 0; i < K_; i++) kb_[i] = nc_[K_ - 1 - i];
    else for (i = 0; i < K_; i++) kb_[i] = nc_[i];
    tab_add(dst_, kb_, w);
}

static void rec(int i, int s, seq_t w) {
    int v, c, bal, p, q;
    if (i == t_) {
        if (s == need_) emit(w);
        return;
    }
    v = ci_[i]; c = cv_[i]; bal = v - B_;
    for (p = 0; p <= c; p++) {
        if (p && bal - 1 < -L_) break;
        for (q = 0; q + p <= c; q++) {
            int r = c - p - q, s2, d;
            if (q && bal + 1 > L_) break;
            if (r && (bal > L_ || bal < -L_)) continue;
            s2 = s + p - q;
            d = need_ - s2;
            if (d > suf_[i + 1] || -d > suf_[i + 1]) continue;
            nc_[v - 1] += (uint8_t)p; nc_[v + 1] += (uint8_t)q; nc_[v] += (uint8_t)r;
            rec(i + 1, s2, w * cb(c, p) * cb(c - p, q));
            nc_[v - 1] -= (uint8_t)p; nc_[v + 1] -= (uint8_t)q; nc_[v] -= (uint8_t)r;
        }
    }
}

#define MEMO_N 256
static seq_t memo_[MEMO_N];
static bool have_[MEMO_N];

seq_t a(idx_t n) {
    int N, i, j, m;
    long long s;
    tab_t cur, nxt;
    seq_t res;
    if (n <= 1) return 1;
    if (n < MEMO_N && have_[n]) return memo_[n];
    N = (int)n;
    K_ = 2 * N + 3; B_ = N + 1;
    nb_ = N;
    Cb_ = alloc_zero((idx_t)((N + 1) * (N + 1)));
    for (i = 0; i <= N; i++) {
        Cb_[(size_t)i * (N + 1)] = 1;
        for (j = 1; j <= i; j++)
            Cb_[(size_t)i * (N + 1) + j] = Cb_[(size_t)(i - 1) * (N + 1) + j - 1]
                                          + Cb_[(size_t)(i - 1) * (N + 1) + j];
    }
    cv_ = (int *)malloc(sizeof(int) * (size_t)K_);
    ci_ = (int *)malloc(sizeof(int) * (size_t)K_);
    suf_ = (int *)malloc(sizeof(int) * (size_t)(K_ + 1));
    nc_ = (uint8_t *)malloc((size_t)K_);
    kb_ = (uint8_t *)malloc((size_t)K_);
    uint8_t *c = (uint8_t *)malloc((size_t)K_);

    tab_init(&cur);
    memset(c, 0, (size_t)K_);
    c[B_] = (uint8_t)N;
    tab_add(&cur, c, (seq_t)1);

    for (m = N; m >= 2; m--) {
        tab_init(&nxt);
        dst_ = &nxt;
        L_ = m - 2;
        for (s = 0; s < cur.cnt; s++) {
            int best = -1, bal;
            seq_t val = cur.vals[s];
            if (val == 0) continue;
            memcpy(c, cur.keys + (size_t)s * K_, (size_t)K_);
            for (i = 0; i < K_; i++) {
                if (c[i]) {
                    int ab = i - B_ < 0 ? B_ - i : i - B_;
                    int bb = best < 0 ? -1 : (best - B_ < 0 ? B_ - best : best - B_);
                    if (ab > bb) best = i;
                }
            }
            c[best]--;
            bal = best - B_;
            need_ = -bal;
            t_ = 0;
            for (i = 0; i < K_; i++)
                if (c[i]) { ci_[t_] = i; cv_[t_] = c[i]; t_++; }
            suf_[t_] = 0;
            for (i = t_ - 1; i >= 0; i--) suf_[i] = suf_[i + 1] + cv_[i];
            if (need_ > suf_[0] || -need_ > suf_[0]) continue;
            memset(nc_, 0, (size_t)K_);
            rec(0, 0, val);
        }
        tab_free(&cur);
        cur = nxt;
    }

    res = 0;
    for (s = 0; s < cur.cnt; s++) res += cur.vals[s];
    tab_free(&cur);

    free(c); free(cv_); free(ci_); free(suf_); free(nc_); free(kb_);
    release(Cb_, (idx_t)((N + 1) * (N + 1)));
    if (n < MEMO_N) { memo_[n] = res; have_[n] = true; }
    return res;
}
