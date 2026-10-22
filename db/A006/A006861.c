/*
 * A006861  Number of directed site animals on hexagonal lattice.
 * https://oeis.org/A006861
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   no closed form is used. The code counts directed site animals on
 *            the honeycomb lattice level by level with memoization. Each site
 *            of the current two-successor level T has two one-successor
 *            children; each child is absent, present, or present together
 *            with its own child, which then belongs to the n...
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   alloc_zero, canon, hsh, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/canon.h"
#include "../utils/hsh.h"

#define KMAX 64

static seq_t *binc = NULL;

typedef struct { uint64_t key; long b; seq_t *p; } memo_ent;
static memo_ent *tab = NULL;
static size_t tcap = 0, tcnt = 0;

static void tab_grow(void) {
    size_t ncap = tcap ? tcap * 2 : 4096, i;
    memo_ent *nt = (memo_ent *)calloc(ncap, sizeof(memo_ent));
    for (i = 0; i < tcap; i++) {
        if (tab[i].key != 0) {
            size_t j = hsh(tab[i].key) & (ncap - 1);
            while (nt[j].key != 0) j = (j + 1) & (ncap - 1);
            nt[j] = tab[i];
        }
    }
    free(tab);
    tab = nt; tcap = ncap;
}

static size_t find_slot(uint64_t key) {
    size_t i;
    if (tcap == 0) tab_grow();
    i = hsh(key) & (tcap - 1);
    while (tab[i].key != 0 && tab[i].key != key) i = (i + 1) & (tcap - 1);
    if (tab[i].key == 0) {
        if (2 * (tcnt + 1) > tcap) {
            tab_grow();
            i = hsh(key) & (tcap - 1);
            while (tab[i].key != 0) i = (i + 1) & (tcap - 1);
        }
        tab[i].key = key; tab[i].b = -1; tab[i].p = NULL;
        tcnt++;
    }
    return i;
}

typedef struct { seq_t *res; long b; int k; int pos[KMAX]; } ctx_t;

static seq_t *F(uint64_t key, long b);

static void sub_rec(ctx_t *cx, int i, uint64_t mask, int c) {
    if (i == cx->k) {
        long nb, off, s, l, lmax;
        int j;
        seq_t *q;
        if (c == 0) return;
        nb = cx->b - 2L * c;
        j = cx->k - c;
        q = F(canon(mask), nb);
        off = 2L * c;
        for (s = 0; s <= nb; s++) {
            if (q[s] == 0) continue;
            lmax = (long)j < nb - s ? (long)j : nb - s;
            for (l = 0; l <= lmax; l++)
                cx->res[off + s + l] += binc[j * (KMAX + 1) + l] * q[s];
        }
        return;
    }
    sub_rec(cx, i + 1, mask, c);
    if (2L * (c + 1) <= cx->b)
        sub_rec(cx, i + 1, mask | (1ULL << cx->pos[i]), c + 1);
}

static seq_t *F(uint64_t key, long b) {
    size_t i = find_slot(key);
    ctx_t cx;
    uint64_t N;
    int t;
    long l;
    if (tab[i].b >= b) return tab[i].p;
    N = key | (key << 1);
    cx.k = 0;
    for (t = 0; t < 64; t++)
        if ((N >> t) & 1ULL) cx.pos[cx.k++] = t;
    cx.b = b;
    cx.res = alloc_zero((idx_t)b + 1);
    for (l = 0; l <= cx.k && l <= b; l++)
        cx.res[l] += binc[cx.k * (KMAX + 1) + l];
    sub_rec(&cx, 0, 0, 0);
    i = find_slot(key);
    if (tab[i].p != NULL) release(tab[i].p, (idx_t)tab[i].b + 1);
    tab[i].p = cx.res;
    tab[i].b = b;
    return cx.res;
}

static seq_t *vals = NULL;
static idx_t have = 0;

seq_t a(idx_t n) {
    if (binc == NULL) {
        int j, l;
        binc = alloc_zero((idx_t)(KMAX + 1) * (KMAX + 1));
        for (j = 0; j <= KMAX; j++) {
            binc[j * (KMAX + 1)] = 1;
            for (l = 1; l <= j; l++)
                binc[j * (KMAX + 1) + l] = binc[(j - 1) * (KMAX + 1) + l - 1]
                                         + (l <= j - 1 ? binc[(j - 1) * (KMAX + 1) + l] : (seq_t)0);
        }
    }
    if (n > have) {
        seq_t *p = F(1ULL, (long)(n - 1));
        seq_t *nv = alloc_zero(n);
        idx_t i;
        for (i = 0; i < n; i++) nv[i] = p[i];
        if (vals != NULL) release(vals, have);
        vals = nv;
        have = n;
    }
    return vals[n - 1];
}
