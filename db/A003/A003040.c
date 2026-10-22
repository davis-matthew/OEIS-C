/*
 * A003040  Highest degree of an irreducible representation of symmetric group
 *          S_n of degree n.
 * https://oeis.org/A003040
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max over partitions lambda of n of f^lambda =
 *            n!/prod(hook lengths) (hook length formula); computed by a beam
 *            search growing partitions one cell at a time, ranked by log
 *            f^lambda, with exact prime-factored evaluation of the best
 *            candidates
 * Range:     u8 n <= 9, u16 n <= 13, u32 n <= 21, u64 n <= 35, u128 n <= 59,
 *            big any n
 * Helpers:   a3040_conj, a3040_exact, a3040_hash
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../utils/a3040_conj.h"
#include "../utils/a3040_exact.h"
#include "../utils/a3040_hash.h"

#define A3040_BEAM 300
#define A3040_KEEP 8

typedef struct { int r; int *p; double lg; } a3040_part;
typedef struct { int nc; int rs[A3040_KEEP]; int *ps[A3040_KEEP]; } a3040_store;

static a3040_part *a3040_cur = NULL;
static int a3040_cnt = 0;
static idx_t a3040_level = 0;
static a3040_store *a3040_st = NULL;
static idx_t a3040_cap = 0;

static int a3040_cmp(const void *x, const void *y) {
    double u = ((const a3040_part *)x)->lg, v = ((const a3040_part *)y)->lg;
    return (u < v) - (u > v);
}

static void a3040_record(int m) {
    if ((idx_t)m >= a3040_cap) {
        idx_t nc = a3040_cap ? a3040_cap : 16;
        while (nc <= (idx_t)m) nc *= 2;
        a3040_st = (a3040_store *)realloc(a3040_st, (size_t)nc * sizeof(a3040_store));
        for (idx_t i = a3040_cap; i < nc; i++) a3040_st[i].nc = 0;
        a3040_cap = nc;
    }
    a3040_store *S = &a3040_st[m];
    double best = a3040_cur[0].lg;
    double tol = 1e-6 + 1e-10 * fabs(best);
    S->nc = 0;
    for (int k = 0; k < a3040_cnt && S->nc < A3040_KEEP; k++) {
        if (a3040_cur[k].lg < best - tol) break;
        int r = a3040_cur[k].r;
        int *q = (int *)malloc((size_t)r * sizeof(int));
        memcpy(q, a3040_cur[k].p, (size_t)r * sizeof(int));
        S->rs[S->nc] = r;
        S->ps[S->nc] = q;
        S->nc++;
    }
}

static void a3040_step(void) {
    int m = (int)a3040_level;
    int maxc = 0;
    for (int k = 0; k < a3040_cnt; k++) maxc += a3040_cur[k].r + 1;
    a3040_part *ch = (a3040_part *)malloc((size_t)maxc * sizeof(a3040_part));
    int nch = 0;
    size_t hs = 1;
    while (hs < (size_t)maxc * 2 + 2) hs <<= 1;
    int *tab = (int *)malloc(hs * sizeof(int));
    for (size_t i = 0; i < hs; i++) tab[i] = -1;
    double lnew = log((double)(m + 1));

    for (int k = 0; k < a3040_cnt; k++) {
        a3040_part *P = &a3040_cur[k];
        int *conj = (int *)malloc((size_t)(P->p[0] + 2) * sizeof(int));
        a3040_conj(P->r, P->p, conj);
        conj[P->p[0]] = 0;
        for (int i = 0; i <= P->r; i++) {
            int j = (i < P->r) ? P->p[i] : 0;
            if (i > 0 && P->p[i - 1] <= j) continue;
            double d = lnew;
            for (int c = 0; c < j; c++) {
                int h = P->p[i] - c + conj[c] - i - 1;
                d -= log((double)(h + 1) / (double)h);
            }
            for (int r = 0; r < i; r++) {
                int h = P->p[r] - j + conj[j] - r - 1;
                d -= log((double)(h + 1) / (double)h);
            }
            int nr = (i < P->r) ? P->r : P->r + 1;
            int *q = (int *)malloc((size_t)nr * sizeof(int));
            for (int r = 0; r < P->r; r++) q[r] = P->p[r];
            if (i < P->r) q[i]++; else q[i] = 1;
            unsigned long long h = a3040_hash(q, nr);
            size_t pos = (size_t)(h & (hs - 1));
            int dup = 0;
            while (tab[pos] != -1) {
                a3040_part *Q = &ch[tab[pos]];
                if (Q->r == nr && memcmp(Q->p, q, (size_t)nr * sizeof(int)) == 0) { dup = 1; break; }
                pos = (pos + 1) & (hs - 1);
            }
            if (dup) { free(q); continue; }
            tab[pos] = nch;
            ch[nch].r = nr; ch[nch].p = q; ch[nch].lg = P->lg + d;
            nch++;
        }
        free(conj);
    }
    free(tab);
    for (int k = 0; k < a3040_cnt; k++) free(a3040_cur[k].p);
    free(a3040_cur);
    qsort(ch, (size_t)nch, sizeof(a3040_part), a3040_cmp);
    int keep = nch < A3040_BEAM ? nch : A3040_BEAM;
    for (int k = keep; k < nch; k++) free(ch[k].p);
    a3040_cur = ch;
    a3040_cnt = keep;
    a3040_level = (idx_t)(m + 1);
    a3040_record(m + 1);
}

seq_t a(idx_t n) {
    if (a3040_level == 0) {
        a3040_cur = (a3040_part *)malloc(sizeof(a3040_part));
        a3040_cur[0].r = 1;
        a3040_cur[0].p = (int *)malloc(sizeof(int));
        a3040_cur[0].p[0] = 1;
        a3040_cur[0].lg = 0.0;
        a3040_cnt = 1;
        a3040_level = 1;
        a3040_record(1);
    }
    while (a3040_level < n) a3040_step();
    a3040_store *S = &a3040_st[n];
    seq_t mx = 0;
    for (int k = 0; k < S->nc; k++) {
        seq_t v = a3040_exact(S->rs[k], S->ps[k], (int)n);
        if (v > mx) mx = v;
    }
    return mx;
}
