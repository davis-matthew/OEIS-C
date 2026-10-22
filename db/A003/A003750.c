/*
 * A003750  Number of Hamiltonian paths in K_5 X P_n.
 * https://oeis.org/A003750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   no closed form used; a(n) is computed with a transfer matrix
 *            over the columns of K_5 X P_n. The state between columns records
 *            which vertical edges are used and how the path fragments to the
 *            left join them. States are reduced modulo the S_5 symmetry to
 *            (number of paired fragments, number of fragme...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   tm_find
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/tm_find.h"

static int R_tm[9][9];
static int F_tm[9];
static bool tm_ready = false;

static void tm_build(void) {
    int eu[10], ev[10], ne = 0;
    for (int u = 0; u < 5; u++)
        for (int w = u + 1; w < 5; w++) { eu[ne] = u; ev[ne] = w; ne++; }

    for (int t = 0; t < 9; t++) {
        for (int k = 0; k < 9; k++) R_tm[t][k] = 0;
        F_tm[t] = 0;
    }

    for (int t = 0; t < 9; t++) {
        int p = t / 3, d = t % 3;
        if (2 * p + d > 5) continue;
        int lab[5] = {0, 0, 0, 0, 0};
        int pos = 0;
        for (int k = 0; k < p; k++) {
            lab[pos] = 2 + pos + 1;
            lab[pos + 1] = 2 + pos;
            pos += 2;
        }
        for (int k = 0; k < d; k++) lab[pos++] = 1;

        for (int E = 0; E < 1024; E++) {
            int edeg[5] = {0, 0, 0, 0, 0};
            for (int e = 0; e < 10; e++)
                if ((E >> e) & 1) { edeg[eu[e]]++; edeg[ev[e]]++; }
            for (int O = 0; O < 32; O++) {
                int deg[5];
                bool ok = true;
                for (int i = 0; i < 5; i++) {
                    deg[i] = (lab[i] != 0) + ((O >> i) & 1) + edeg[i];
                    if (deg[i] < 1 || deg[i] > 2) { ok = false; break; }
                }
                if (!ok) continue;

                int par[5];
                for (int i = 0; i < 5; i++) par[i] = i;
                for (int i = 0; i < 5; i++) {
                    if (lab[i] >= 2) {
                        int j = lab[i] - 2;
                        if (i < j) {
                            int ri = tm_find(par, i), rj = tm_find(par, j);
                            par[ri] = rj;
                        }
                    }
                }
                for (int e = 0; e < 10 && ok; e++) {
                    if (!((E >> e) & 1)) continue;
                    int ru = tm_find(par, eu[e]), rv = tm_find(par, ev[e]);
                    if (ru == rv) ok = false;
                    else par[ru] = rv;
                }
                if (!ok) continue;

                int term[5] = {0, 0, 0, 0, 0}, outs[5] = {0, 0, 0, 0, 0};
                int totterm = 0;
                for (int i = 0; i < 5; i++) {
                    int r = tm_find(par, i);
                    if (lab[i] == 1) { term[r]++; totterm++; }
                    if (deg[i] == 1) { term[r]++; totterm++; }
                    if ((O >> i) & 1) outs[r]++;
                }
                if (totterm > 2) continue;

                int ncomp = 0, np = 0, nd = 0;
                for (int i = 0; i < 5; i++) {
                    if (tm_find(par, i) != i) continue;
                    ncomp++;
                    if (term[i] + outs[i] != 2) ok = false;
                    if (outs[i] == 2) np++;
                    else if (outs[i] == 1) nd++;
                    else if (O != 0) ok = false;
                }
                if (!ok) continue;

                if (O == 0) {
                    if (ncomp == 1 && totterm == 2) F_tm[t]++;
                } else {
                    R_tm[t][np * 3 + nd]++;
                }
            }
        }
    }
    tm_ready = true;
}

seq_t a(idx_t n) {
    if (!tm_ready) tm_build();
    seq_t v[9], w[9];
    for (int k = 0; k < 9; k++) v[k] = 0;
    v[0] = 1;
    for (idx_t s = 1; s < n; s++) {
        for (int k = 0; k < 9; k++) w[k] = 0;
        for (int x = 0; x < 9; x++) {
            if (v[x] == 0) continue;
            for (int y = 0; y < 9; y++)
                if (R_tm[x][y] != 0) w[y] += v[x] * (seq_t)R_tm[x][y];
        }
        for (int k = 0; k < 9; k++) v[k] = w[k];
    }
    seq_t res = 0;
    for (int k = 0; k < 9; k++)
        if (F_tm[k] != 0) res += v[k] * (seq_t)F_tm[k];
    return res;
}
