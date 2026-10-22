/*
 * A006016  The nim value for the game of Sym with n tails and 1 head.
 * https://oeis.org/A006016
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = mex over l = 0..n of the sets { a(l) XOR (XOR of any
 *            subset of { a(i) XOR a(l+n-i) : l < i < (l+n)/2 } together with
 *            a((l+n)/2) when l+n is even and l < n) } (a(l) is taken as 0
 *            when l = n); a(0) = 1. Coin-turning game: a symmetric move on
 *            [l,n] leaves heads whose nim values XOR together;...
 * Range:     u8 n <= 15, u16 n <= 25, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   bitlen_w, getbit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/bitlen_w.h"
#include "../utils/getbit.h"

static uint64_t **Gw = NULL;
static int *Gn = NULL;
static long long Gcnt = 0, Gcap = 0;

typedef struct {
    uint64_t *c;
    int *prow;
    uint64_t *rows;
    int nr;
    int lownp;
} Coset;

static Coset *CS;
static int CW;

static void xor_g(uint64_t *dst, long long i) {
    for (int k = 0; k < Gn[i]; k++) dst[k] ^= Gw[i][k];
}

static void coset_insert(Coset *q, uint64_t *t, int nbits, int W) {
    for (int b = nbits - 1; b >= 0; b--) {
        if (!getbit(t, b)) continue;
        if (q->prow[b] >= 0) {
            uint64_t *r = q->rows + (size_t)q->prow[b] * W;
            for (int k = 0; k < W; k++) t[k] ^= r[k];
        } else {
            memcpy(q->rows + (size_t)q->nr * W, t, (size_t)W * sizeof(uint64_t));
            q->prow[b] = q->nr++;
            return;
        }
    }
}

static int dfs(int b, int cnt, const int *ids, const uint64_t *acc, uint64_t *res) {
    int W = CW;
    if (cnt == 0) return 1;
    if (b < 0) return 0;
    for (int i = 0; i < cnt; i++)
        if (CS[ids[i]].lownp > b) return 0;
    int *nids = (int *)malloc((size_t)cnt * sizeof(int));
    uint64_t *nacc = (uint64_t *)malloc((size_t)cnt * W * sizeof(uint64_t));
    for (int v = 0; v <= 1; v++) {
        int ncnt = 0;
        for (int i = 0; i < cnt; i++) {
            Coset *q = &CS[ids[i]];
            const uint64_t *src = acc + (size_t)i * W;
            uint64_t *dst = nacc + (size_t)ncnt * W;
            if (q->prow[b] >= 0) {
                memcpy(dst, src, (size_t)W * sizeof(uint64_t));
                if (v) {
                    uint64_t *r = q->rows + (size_t)q->prow[b] * W;
                    for (int k = 0; k < W; k++) dst[k] ^= r[k];
                }
                nids[ncnt++] = ids[i];
            } else if (getbit(src, b) == v) {
                memcpy(dst, src, (size_t)W * sizeof(uint64_t));
                nids[ncnt++] = ids[i];
            }
        }
        if (dfs(b - 1, ncnt, nids, nacc, res)) {
            if (v) res[b >> 6] |= (uint64_t)1 << (b & 63);
            free(nids);
            free(nacc);
            return 1;
        }
    }
    free(nids);
    free(nacc);
    return 0;
}

static void compute_next(void) {
    long long m = Gcnt;
    int B = 0;
    for (long long i = 0; i < m; i++) {
        int bl = bitlen_w(Gw[i], Gn[i]);
        if (bl > B) B = bl;
    }
    int W = B / 64 + 1;
    int nbits = B + 1;
    Coset *cs = (Coset *)malloc((size_t)(m + 1) * sizeof(Coset));
    uint64_t *tmp = (uint64_t *)malloc((size_t)W * sizeof(uint64_t));
    for (long long l = 0; l <= m; l++) {
        Coset *q = &cs[l];
        q->c = (uint64_t *)calloc((size_t)W, sizeof(uint64_t));
        q->prow = (int *)malloc((size_t)nbits * sizeof(int));
        for (int b = 0; b < nbits; b++) q->prow[b] = -1;
        long long ng = (m - l) / 2 + 2;
        q->rows = (uint64_t *)calloc((size_t)ng * W, sizeof(uint64_t));
        q->nr = 0;
        if (l < m) xor_g(q->c, l);
        for (long long i = l + 1; 2 * i < l + m; i++) {
            memset(tmp, 0, (size_t)W * sizeof(uint64_t));
            xor_g(tmp, i);
            xor_g(tmp, l + m - i);
            coset_insert(q, tmp, nbits, W);
        }
        if (m > l && (l + m) % 2 == 0) {
            memset(tmp, 0, (size_t)W * sizeof(uint64_t));
            xor_g(tmp, (l + m) / 2);
            coset_insert(q, tmp, nbits, W);
        }
        /* full reduction to reduced row echelon form */
        for (int p = 0; p < nbits; p++) {
            if (q->prow[p] < 0) continue;
            uint64_t *v = q->rows + (size_t)q->prow[p] * W;
            for (int r = p - 1; r >= 0; r--) {
                if (q->prow[r] >= 0 && getbit(v, r)) {
                    uint64_t *u = q->rows + (size_t)q->prow[r] * W;
                    for (int k = 0; k < W; k++) v[k] ^= u[k];
                }
            }
        }
        for (int r = nbits - 1; r >= 0; r--) {
            if (q->prow[r] >= 0 && getbit(q->c, r)) {
                uint64_t *u = q->rows + (size_t)q->prow[r] * W;
                for (int k = 0; k < W; k++) q->c[k] ^= u[k];
            }
        }
        q->lownp = 0;
        while (q->lownp < nbits && q->prow[q->lownp] >= 0) q->lownp++;
    }
    int cnt = (int)(m + 1);
    int *ids = (int *)malloc((size_t)cnt * sizeof(int));
    uint64_t *acc = (uint64_t *)malloc((size_t)cnt * W * sizeof(uint64_t));
    for (int i = 0; i < cnt; i++) {
        ids[i] = i;
        memcpy(acc + (size_t)i * W, cs[i].c, (size_t)W * sizeof(uint64_t));
    }
    uint64_t *res = (uint64_t *)calloc((size_t)W, sizeof(uint64_t));
    CS = cs;
    CW = W;
    dfs(B, cnt, ids, acc, res);
    free(ids);
    free(acc);
    free(tmp);
    for (long long l = 0; l <= m; l++) {
        free(cs[l].c);
        free(cs[l].prow);
        free(cs[l].rows);
    }
    free(cs);
    if (Gcnt >= Gcap) {
        Gcap = Gcap ? Gcap * 2 : 64;
        Gw = (uint64_t **)realloc(Gw, (size_t)Gcap * sizeof(uint64_t *));
        Gn = (int *)realloc(Gn, (size_t)Gcap * sizeof(int));
    }
    Gw[Gcnt] = res;
    Gn[Gcnt] = W;
    Gcnt++;
}

seq_t a(idx_t n) {
    while ((idx_t)Gcnt <= n) compute_next();
    const uint64_t *v = Gw[n];
    int nb = Gn[n] * 8;
    seq_t r = 0;
    for (int k = nb - 1; k >= 0; k--) {
        uint64_t byte = (v[k >> 3] >> (8 * (k & 7))) & 255;
        r <<= 8;
        r |= (seq_t)byte;
    }
    return r;
}
