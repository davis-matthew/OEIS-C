/*
 * A007825  Number of n step self-avoiding walks on 3 X infinity grid starting
 *          from (0,1).
 * https://oeis.org/A007825
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   no closed form is used; the code counts the walks with a
 *            column-by-column transfer matrix. Each walk is cut into columns,
 *            and the state at each cut records which of the 3 rows carry the
 *            path and how those strands connect on the left. A strand either
 *            belongs to an arc that returns, or leads back t...
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 31, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/grow_zero.h"

static int tr_n[64];
static int tr_t[64][256];
static int tr_d[64][256];
static bool tr_ready = false;

static int uf_find(int *par, int x) {
    while (par[x] != x) x = par[x];
    return x;
}

static void build_transitions(void) {
    for (int s = 0; s < 64; s++) {
        int p[3];
        p[0] = s & 3; p[1] = (s >> 2) & 3; p[2] = (s >> 4) & 3;
        tr_n[s] = 0;
        for (int v = 0; v < 4; v++) {
            for (int h = 0; h < 8; h++) {
                int v01 = v & 1, v12 = (v >> 1) & 1;
                int hh[3];
                hh[0] = h & 1; hh[1] = (h >> 1) & 1; hh[2] = (h >> 2) & 1;
                if (s == 0 && h == 0 && v == 0) continue;
                int deg[3];
                bool ok = true;
                for (int r = 0; r < 3; r++) {
                    int vert = (r == 0) ? v01 : (r == 1 ? v01 + v12 : v12);
                    deg[r] = (p[r] != 0) + hh[r] + vert;
                    if (deg[r] > 2) ok = false;
                }
                if (!ok) continue;
                int ep[3], ne = 0;
                for (int r = 0; r < 3; r++) if (deg[r] == 1) ep[ne++] = r;
                int d = v01 + v12 + hh[0] + hh[1] + hh[2];
                for (int lab = 0; lab < (1 << ne); lab++) {
                    int label[3];
                    label[0] = label[1] = label[2] = 0;
                    int cS = 0, cE = 0;
                    bool good = true;
                    for (int r = 0; r < 3; r++) {
                        if (p[r] == 2) cS++;
                        if (p[r] == 3) cE++;
                    }
                    for (int k = 0; k < ne; k++) {
                        int r = ep[k];
                        label[r] = ((lab >> k) & 1) ? 2 : 3;
                        if (label[r] == 2) { cS++; if (r != 1) good = false; }
                        else cE++;
                    }
                    if (cS > 1 || cE > 1) good = false;
                    if (!good) continue;
                    int par[3];
                    par[0] = 0; par[1] = 1; par[2] = 2;
                    if (v01) { int x = uf_find(par, 0), y = uf_find(par, 1); par[x] = y; }
                    if (v12) { int x = uf_find(par, 1), y = uf_find(par, 2); if (x != y) par[x] = y; }
                    {
                        int ar[3], na = 0;
                        for (int r = 0; r < 3; r++) if (p[r] == 1) ar[na++] = r;
                        if (na == 2) {
                            int x = uf_find(par, ar[0]), y = uf_find(par, ar[1]);
                            if (x == y) good = false; /* would close a cycle via vertical edges */
                            else par[x] = y;
                        } else if (na != 0) good = false;
                    }
                    if (!good) continue;
                    int nplug[3], nS[3], nE[3];
                    bool isroot[3];
                    for (int r = 0; r < 3; r++) { nplug[r] = nS[r] = nE[r] = 0; isroot[r] = false; }
                    for (int r = 0; r < 3; r++) {
                        if (deg[r] == 0) continue;
                        int rt = uf_find(par, r);
                        isroot[rt] = true;
                        if (hh[r]) nplug[rt]++;
                        if (p[r] == 2 || label[r] == 2) nS[rt]++;
                        if (p[r] == 3 || label[r] == 3) nE[rt]++;
                    }
                    int ncomp = 0, ncomplete = 0;
                    for (int r = 0; r < 3; r++) {
                        if (!isroot[r]) continue;
                        ncomp++;
                        int ends = nplug[r] + nS[r] + nE[r];
                        if (ends != 2) { good = false; break; }
                        if (nplug[r] == 0) {
                            if (nS[r] != 1 || nE[r] != 1) { good = false; break; }
                            ncomplete++;
                        }
                    }
                    if (!good) continue;
                    if (ncomplete > 0) {
                        if (ncomp != 1) continue;
                        tr_t[s][tr_n[s]] = -1;
                        tr_d[s][tr_n[s]] = d;
                        tr_n[s]++;
                        continue;
                    }
                    int out = 0;
                    for (int r = 0; r < 3; r++) {
                        if (!hh[r]) continue;
                        int rt = uf_find(par, r);
                        int dig;
                        if (nplug[rt] == 2) dig = 1;
                        else if (nS[rt] == 1) dig = 2;
                        else dig = 3;
                        out |= dig << (2 * r);
                    }
                    if (out == 0) continue;
                    tr_t[s][tr_n[s]] = out;
                    tr_d[s][tr_n[s]] = d;
                    tr_n[s]++;
                }
            }
        }
    }
}

static seq_t ring[6][64];
static seq_t *ans = NULL;
static idx_t cap = 0;
static long long done_deg = -1;

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (!tr_ready) {
        build_transitions();
        tr_ready = true;
        ring[0][0] = 1;
    }
    if (n + 6 > cap) {
        idx_t nc = n + 6;
        if (nc < 2 * cap) nc = 2 * cap;
        ans = grow_zero(ans, cap, nc);
        cap = nc;
    }
    while (done_deg < (long long)n) {
        long long j = done_deg + 1;
        int sl = (int)(j % 6);
        for (int s = 0; s < 64; s++) {
            if (ring[sl][s] == 0) continue;
            for (int t = 0; t < tr_n[s]; t++) {
                int d = tr_d[s][t];
                if (tr_t[s][t] < 0) ans[j + d] += ring[sl][s];
                else ring[(j + d) % 6][tr_t[s][t]] += ring[sl][s];
            }
            ring[sl][s] = 0;
        }
        done_deg = j;
    }
    return ans[n];
}
