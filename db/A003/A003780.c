/*
 * A003780  Number of spanning trees with degrees 1 and 3 in P_5 X P_n.
 * https://oeis.org/A003780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   transfer-matrix count over column states (rows with a horizontal
 *            edge to the next column + their connectivity); a(n) = sum over
 *            states of (e0 * T^(n-1))[s] * F[s], where T adds one column with
 *            each vertex degree in {1,3} and no cycles, and F closes the grid
 *            into a single tree
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 21, u64 n <= 37, u128 n <= 41,
 *            big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/grow_zero.h"

#define W 5
#define MAXC 7776

static int par[10];
static int fnd(int x) { while (par[x] != x) x = par[x] = par[par[x]]; return x; }

/* returns -1 if invalid; for fin returns 0 if valid; otherwise returns new state code */
static int step(const int *lab, int vm, int so, int fin) {
    int i, deg, has[10], rl[10], nl = 0, code = 0, mul = 1;
    for (i = 0; i < 10; i++) { par[i] = i; has[i] = 0; rl[i] = 0; }
    for (i = 0; i < W; i++) {
        deg = (lab[i] != 0) + ((vm >> i) & 1) + (i > 0 ? ((vm >> (i - 1)) & 1) : 0) + ((so >> i) & 1);
        if (deg != 1 && deg != 3) return -1;
    }
    for (i = 0; i < W; i++) if (lab[i]) {
        int a = fnd(lab[i] - 1), b = fnd(5 + i);
        if (a == b) return -1;
        par[a] = b;
    }
    for (i = 0; i < W - 1; i++) if ((vm >> i) & 1) {
        int a = fnd(5 + i), b = fnd(6 + i);
        if (a == b) return -1;
        par[a] = b;
    }
    if (fin) {
        for (i = 1; i < W; i++) if (fnd(5 + i) != fnd(5)) return -1;
        return 0;
    }
    for (i = 0; i < W; i++) if ((so >> i) & 1) has[fnd(5 + i)] = 1;
    for (i = 0; i < W; i++) if (!has[fnd(5 + i)]) return -1;
    for (i = 0; i < W; i++) {
        int l = 0;
        if ((so >> i) & 1) {
            int r = fnd(5 + i);
            if (!rl[r]) rl[r] = ++nl;
            l = rl[r];
        }
        code += l * mul;
        mul *= 6;
    }
    return code;
}

static int built = 0, nst = 0;
static int codes[MAXC], idxof[MAXC], fins[MAXC], tstart[MAXC + 1];
static int *ttgt = NULL, *tcnt = NULL, tlen = 0, tcap = 0;

static void build(void) {
    int k, i, vm, so, r, c;
    static int tmp[MAXC], touched[MAXC];
    int nt;
    for (i = 0; i < MAXC; i++) { idxof[i] = -1; tmp[i] = 0; }
    codes[0] = 0; idxof[0] = 0; nst = 1;
    for (k = 0; k < nst; k++) {
        int lab[W];
        c = codes[k];
        for (i = 0; i < W; i++) { lab[i] = c % 6; c /= 6; }
        fins[k] = 0;
        for (vm = 0; vm < 16; vm++) if (step(lab, vm, 0, 1) == 0) fins[k]++;
        nt = 0;
        for (vm = 0; vm < 16; vm++)
            for (so = 1; so < 32; so++) {
                r = step(lab, vm, so, 0);
                if (r < 0) continue;
                if (idxof[r] < 0) { idxof[r] = nst; codes[nst++] = r; }
                if (tmp[idxof[r]]++ == 0) touched[nt++] = idxof[r];
            }
        tstart[k] = tlen;
        for (i = 0; i < nt; i++) {
            if (tlen == tcap) {
                tcap = tcap ? 2 * tcap : 256;
                ttgt = (int *)realloc(ttgt, sizeof(int) * tcap);
                tcnt = (int *)realloc(tcnt, sizeof(int) * tcap);
            }
            ttgt[tlen] = touched[i];
            tcnt[tlen] = tmp[touched[i]];
            tlen++;
            tmp[touched[i]] = 0;
        }
    }
    tstart[nst] = tlen;
    built = 1;
}

static seq_t cur[MAXC], nxt[MAXC];
static seq_t *ans = NULL;
static idx_t alen = 1, acap = 0;

seq_t a(idx_t n) {
    int k, t;
    if (!built) {
        build();
        for (k = 0; k < nst; k++) cur[k] = 0;
        cur[0] = 1;
    }
    while (alen <= n) {
        seq_t s = 0;
        if (alen >= acap) {
            idx_t nc = acap ? 2 * acap : 64;
            while (nc <= alen) nc *= 2;
            ans = grow_zero(ans, acap, nc);
            acap = nc;
        }
        for (k = 0; k < nst; k++) if (fins[k]) s += cur[k] * fins[k];
        ans[alen] = s;
        for (k = 0; k < nst; k++) nxt[k] = 0;
        for (k = 0; k < nst; k++) {
            if (cur[k] == 0) continue;
            for (t = tstart[k]; t < tstart[k + 1]; t++)
                nxt[ttgt[t]] += cur[k] * tcnt[t];
        }
        for (k = 0; k < nst; k++) cur[k] = nxt[k];
        alen++;
    }
    return ans[n];
}
