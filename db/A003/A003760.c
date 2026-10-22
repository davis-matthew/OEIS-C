/*
 * A003760  Number of Hamiltonian paths in D_4 X P_n.
 * https://oeis.org/A003760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of Hamiltonian paths in D_4 X P_n, where D_4 is
 *            the paw graph (triangle a,b,c plus pendant d at a), computed by
 *            a transfer-matrix DP over the layers (states = vertical plugs
 *            with their path connectivity)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   alloc_zero, grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"

static int uf_find(int *p, int x) { while (p[x] != x) x = p[x]; return x; }

/* s: plugs entering the layer from below (2 bits per vertex: 0 none,
   1 fragment ending in a path endpoint, 2/3 paired labels).
   H: in-layer edges used (ab, ac, bc, ad); U: plugs going up. */
static int step(int s, int H, int U, int fin, int *out)
{
    static const int eu[4] = {0, 0, 1, 0}, ev[4] = {1, 2, 2, 3};
    int par[4], ends[4], deg[4], pc[4], lab[4], dg[4];
    int i, j, e, t = 0, nextlab = 2, roots = 0, tot = 0;
    for (i = 0; i < 4; i++) {
        par[i] = i; ends[i] = 0; deg[i] = 0; pc[i] = 0; lab[i] = 0;
        dg[i] = (s >> (2 * i)) & 3;
        if (dg[i]) { deg[i]++; if (dg[i] == 1) ends[i]++; }
    }
    for (i = 0; i < 4; i++)
        for (j = i + 1; j < 4; j++)
            if (dg[i] >= 2 && dg[i] == dg[j]) {
                int ri = uf_find(par, i), rj = uf_find(par, j);
                if (ri == rj) return 0;
                par[ri] = rj; ends[rj] += ends[ri];
            }
    for (e = 0; e < 4; e++)
        if ((H >> e) & 1) {
            int ri = uf_find(par, eu[e]), rj = uf_find(par, ev[e]);
            if (ri == rj) return 0;
            par[ri] = rj; ends[rj] += ends[ri];
            deg[eu[e]]++; deg[ev[e]]++;
        }
    for (i = 0; i < 4; i++) if ((U >> i) & 1) deg[i]++;
    for (i = 0; i < 4; i++) {
        if (deg[i] < 1 || deg[i] > 2) return 0;
        if (deg[i] == 1) ends[uf_find(par, i)]++;
    }
    for (i = 0; i < 4; i++) if ((U >> i) & 1) pc[uf_find(par, i)]++;
    for (i = 0; i < 4; i++)
        if (par[i] == i) {
            roots++;
            tot += ends[i];
            if (ends[i] + pc[i] != 2) return 0;
            if (!fin && pc[i] == 0) return 0;
        }
    if (tot > 2) return 0;
    if (fin) {
        if (U || roots != 1) return 0;
        *out = 0;
        return 1;
    }
    for (i = 0; i < 4; i++)
        if ((U >> i) & 1) {
            int r = uf_find(par, i), d;
            if (ends[r] == 1) d = 1;
            else { if (!lab[r]) lab[r] = nextlab++; d = lab[r]; }
            t |= d << (2 * i);
        }
    *out = t;
    return 1;
}

static int inited = 0;
static int nreach = 0, reachlist[256], F[256];
static int ntr = 0, ts[65536], tt[65536], tm[65536];
static int M[256][256];
static seq_t cur[256], nxt[256];
static seq_t *vals = NULL;
static idx_t cap = 0, layers = 0;

static void init(void)
{
    int reach[256], q[256], qh = 0, qt = 0, i, s, t, H, U;
    for (i = 0; i < 256; i++) { reach[i] = 0; F[i] = 0; }
    reach[0] = 1; q[qt++] = 0;
    while (qh < qt) {
        s = q[qh++];
        for (H = 0; H < 16; H++) {
            for (U = 0; U < 16; U++)
                if (step(s, H, U, 0, &t)) {
                    M[s][t]++;
                    if (!reach[t]) { reach[t] = 1; q[qt++] = t; }
                }
            if (step(s, H, 0, 1, &t)) F[s]++;
        }
    }
    nreach = qt;
    for (i = 0; i < qt; i++) reachlist[i] = q[i];
    for (i = 0; i < qt; i++) {
        s = q[i];
        for (t = 0; t < 256; t++)
            if (M[s][t]) { ts[ntr] = s; tt[ntr] = t; tm[ntr] = M[s][t]; ntr++; }
    }
    for (i = 0; i < 256; i++) { cur[i] = 0; nxt[i] = 0; }
    cur[0] = 1;
    layers = 0;
    inited = 1;
}

seq_t a(idx_t n)
{
    int i, k;
    if (!inited) init();
    if (n < 1) return 0;
    if (n + 1 > cap) {
        idx_t nc = cap ? cap : 64;
        while (nc < n + 1) nc *= 2;
        if (vals == NULL) vals = alloc_zero(nc);
        else vals = grow_zero(vals, cap, nc);
        cap = nc;
    }
    while (layers < n) {
        seq_t v = 0;
        for (i = 0; i < nreach; i++) {
            int s = reachlist[i];
            if (F[s]) v += cur[s] * (seq_t)F[s];
        }
        vals[layers + 1] = v;
        for (i = 0; i < nreach; i++) nxt[reachlist[i]] = 0;
        for (k = 0; k < ntr; k++) nxt[tt[k]] += cur[ts[k]] * (seq_t)tm[k];
        for (i = 0; i < nreach; i++) cur[reachlist[i]] = nxt[reachlist[i]];
        layers++;
    }
    return vals[n];
}
