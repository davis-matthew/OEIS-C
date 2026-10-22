/*
 * A003856  Degrees of irreducible representations of Mathieu group M_12.
 * https://oeis.org/A003856
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   no closed formula. The code builds M_12 as the automorphism
 *            group of the Steiner system S(5,6,12), whose hexads are the
 *            PSL(2,11)-orbit of {oo,1,3,4,5,9}. It finds the conjugacy
 *            classes and the class multiplication coefficients, then uses
 *            Dixon's method mod p to get the central characters. Each d...
 * Range:     u8 n <= 15, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   fld_inv, gcd_ll, key5, mapS, mapT, prime_ll
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/fld_inv.h"
#include "../utils/gcd_ll.h"
#include "../utils/key5.h"
#include "../utils/mapS.h"
#include "../utils/mapT.h"
#include "../utils/prime_ll.h"

#define NPT 12
#define GMAX 95040
#define KEYSZ 248832
#define MC 32

static unsigned char g_el[GMAX][NPT];
static int g_n = 0;
static int g_idx[KEYSZ];
static int g_cls[GMAX];
static bool is_hex[4096];
static int hex_list[4096];
static int nhex = 0;
static int order_pt[NPT], pos_pt[NPT];
static int dl[NPT][140], dl_n[NPT];
static int img[NPT];
static int csize[MC], crep[MC], cinv[MC];
static long long cst[MC][MC][MC];
static long long sb[MC][MC][MC], nsb[MC][MC][MC];
static int sdim[MC], nsdim[MC];
static long long PR;
static int degs[MC];
static int ndeg = 0;
static int inited = 0;

static int apply_mask(int m, int which) {
    int r = 0, i;
    for (i = 0; i < NPT; i++)
        if ((m >> i) & 1) r |= 1 << (which ? mapS(i) : mapT(i));
    return r;
}

static bool build_design(int base) {
    static unsigned char cnt5[4096];
    int h, w, i;
    memset(is_hex, 0, sizeof(is_hex));
    nhex = 0;
    is_hex[base] = true;
    hex_list[nhex++] = base;
    for (h = 0; h < nhex; h++)
        for (w = 0; w < 2; w++) {
            int m2 = apply_mask(hex_list[h], w);
            if (!is_hex[m2]) {
                if (nhex >= 4096) return false;
                is_hex[m2] = true;
                hex_list[nhex++] = m2;
            }
        }
    if (nhex != 132) return false;
    memset(cnt5, 0, sizeof(cnt5));
    for (h = 0; h < nhex; h++)
        for (i = 0; i < NPT; i++)
            if ((hex_list[h] >> i) & 1) {
                int m5 = hex_list[h] & ~(1 << i);
                if (cnt5[m5]++) return false;
            }
    return true;
}

static void search(int d, int used) {
    int q, v, t, i;
    if (d == NPT) {
        if (g_n < GMAX) {
            for (i = 0; i < NPT; i++) g_el[g_n][i] = (unsigned char)img[i];
            g_n++;
        }
        return;
    }
    q = order_pt[d];
    for (v = 0; v < NPT; v++) {
        bool ok = true;
        if ((used >> v) & 1) continue;
        img[q] = v;
        for (t = 0; t < dl_n[d] && ok; t++) {
            int h = dl[d][t], im = 0;
            for (i = 0; i < NPT; i++)
                if ((h >> i) & 1) im |= 1 << img[i];
            if (!is_hex[im]) ok = false;
        }
        if (ok) search(d + 1, used | (1 << v));
    }
}

static long long pmod(long long b, long long e) {
    long long r = 1;
    b %= PR;
    if (b < 0) b += PR;
    while (e > 0) {
        if (e & 1) r = r * b % PR;
        b = b * b % PR;
        e >>= 1;
    }
    return r;
}
static long long minv(long long x) { return pmod(x, PR - 2); }

static int nullspace(long long A[MC][MC], int rows, int cols, long long out[MC][MC]) {
    int pivc[MC], isp[MC];
    int r = 0, c, i, k, cnt = 0, f, rr;
    for (c = 0; c < cols; c++) isp[c] = 0;
    for (c = 0; c < cols && r < rows; c++) {
        int pv = -1;
        long long iv;
        for (i = r; i < rows; i++) if (A[i][c] != 0) { pv = i; break; }
        if (pv < 0) continue;
        if (pv != r)
            for (k = 0; k < cols; k++) { long long t = A[pv][k]; A[pv][k] = A[r][k]; A[r][k] = t; }
        iv = minv(A[r][c]);
        for (k = 0; k < cols; k++) A[r][k] = A[r][k] * iv % PR;
        for (i = 0; i < rows; i++) {
            if (i != r && A[i][c] != 0) {
                long long fct = A[i][c];
                for (k = 0; k < cols; k++) {
                    A[i][k] = (A[i][k] - fct * A[r][k]) % PR;
                    if (A[i][k] < 0) A[i][k] += PR;
                }
            }
        }
        pivc[r] = c;
        isp[c] = 1;
        r++;
    }
    for (f = 0; f < cols; f++) {
        if (isp[f]) continue;
        for (k = 0; k < cols; k++) out[cnt][k] = 0;
        out[cnt][f] = 1;
        for (rr = 0; rr < r; rr++) out[cnt][pivc[rr]] = (PR - A[rr][f]) % PR;
        cnt++;
    }
    return cnt;
}

static void init_all(void) {
    int bases[2];
    int bi, base = -1, i, h, e, g, c, k, j, s, t;
    int nc = 0, ident = -1;
    long long expo = 1, G, bound, p;
    int ns;

    inited = 1;
    bases[0] = (1 << 11) | (1 << 1) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 9);
    bases[1] = (1 << 0) | (1 << 1) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 9);
    for (bi = 0; bi < 2; bi++)
        if (build_design(bases[bi])) { base = bases[bi]; break; }
    if (base < 0) return;

    k = 0;
    for (i = 0; i < NPT; i++) if ((base >> i) & 1) order_pt[k++] = i;
    for (i = 0; i < NPT; i++) if (!((base >> i) & 1)) order_pt[k++] = i;
    for (i = 0; i < NPT; i++) pos_pt[order_pt[i]] = i;
    for (i = 0; i < NPT; i++) dl_n[i] = 0;
    for (h = 0; h < nhex; h++) {
        int mp = 0;
        for (i = 0; i < NPT; i++)
            if (((hex_list[h] >> i) & 1) && pos_pt[i] > mp) mp = pos_pt[i];
        dl[mp][dl_n[mp]++] = hex_list[h];
    }
    g_n = 0;
    search(0, 0);

    for (i = 0; i < KEYSZ; i++) g_idx[i] = -1;
    for (e = 0; e < g_n; e++) {
        int pp[5], isid = 1;
        for (i = 0; i < 5; i++) pp[i] = g_el[e][i];
        g_idx[key5(pp)] = e;
        g_cls[e] = -1;
        for (i = 0; i < NPT; i++) if (g_el[e][i] != i) isid = 0;
        if (isid) ident = e;
    }
    if (ident < 0) return;

    for (bi = -1; bi < g_n; bi++) {
        int ee = (bi < 0) ? ident : bi;
        const unsigned char *x;
        if (g_cls[ee] >= 0) continue;
        if (nc >= MC) return;
        c = nc++;
        crep[c] = ee;
        csize[c] = 0;
        x = g_el[ee];
        for (g = 0; g < g_n; g++) {
            int r[NPT], kk;
            const unsigned char *gg = g_el[g];
            for (i = 0; i < NPT; i++) r[gg[i]] = gg[x[i]];
            kk = g_idx[key5(r)];
            if (kk >= 0 && g_cls[kk] < 0) { g_cls[kk] = c; csize[c]++; }
        }
    }

    for (c = 0; c < nc; c++) {
        const unsigned char *x = g_el[crep[c]];
        int xi[NPT], y[NPT], yn[NPT], ord = 1, isid;
        for (i = 0; i < NPT; i++) xi[x[i]] = i;
        cinv[c] = g_cls[g_idx[key5(xi)]];
        for (i = 0; i < NPT; i++) y[i] = x[i];
        for (;;) {
            isid = 1;
            for (i = 0; i < NPT; i++) if (y[i] != i) isid = 0;
            if (isid) break;
            for (i = 0; i < NPT; i++) yn[i] = x[y[i]];
            for (i = 0; i < NPT; i++) y[i] = yn[i];
            ord++;
        }
        expo = expo / gcd_ll(expo, ord) * ord;
    }

    memset(cst, 0, sizeof(cst));
    for (k = 0; k < nc; k++) {
        const unsigned char *z = g_el[crep[k]];
        for (e = 0; e < g_n; e++) {
            const unsigned char *x = g_el[e];
            int xi[NPT], y5[5];
            for (i = 0; i < NPT; i++) xi[x[i]] = i;
            for (i = 0; i < 5; i++) y5[i] = z[xi[i]];
            cst[g_cls[e]][g_cls[g_idx[key5(y5)]]][k]++;
        }
    }

    G = g_n;
    bound = 2;
    while (bound * bound <= G) bound++;
    bound = 2 * bound + 2;
    for (p = expo + 1;; p += expo)
        if (p > bound && prime_ll(p)) break;
    PR = p;

    ns = 1;
    sdim[0] = nc;
    for (t = 0; t < nc; t++)
        for (i = 0; i < nc; i++) sb[0][t][i] = (t == i) ? 1 : 0;

    for (j = 1; j < nc; j++) {
        int nn = 0;
        for (s = 0; s < ns; s++) {
            int m = sdim[s], got = 0, start = nn;
            long long lam;
            static long long Mw[MC][MC], U[MC][MC], nv[MC][MC];
            if (m == 1) {
                nsdim[nn] = 1;
                for (i = 0; i < nc; i++) nsb[nn][0][i] = sb[s][0][i];
                nn++;
                continue;
            }
            for (t = 0; t < m; t++)
                for (i = 0; i < nc; i++) {
                    long long acc = 0;
                    for (k = 0; k < nc; k++)
                        acc = (acc + (cst[i][j][k] % PR) * sb[s][t][k]) % PR;
                    Mw[t][i] = acc;
                }
            for (lam = 0; lam < PR && got < m; lam++) {
                int nd, q;
                for (i = 0; i < nc; i++)
                    for (t = 0; t < m; t++) {
                        long long v = (Mw[t][i] - lam * sb[s][t][i]) % PR;
                        if (v < 0) v += PR;
                        U[i][t] = v;
                    }
                nd = nullspace(U, nc, m, nv);
                if (nd == 0) continue;
                if (nn >= MC) break;
                nsdim[nn] = nd;
                for (q = 0; q < nd; q++)
                    for (i = 0; i < nc; i++) {
                        long long acc = 0;
                        for (t = 0; t < m; t++) acc = (acc + nv[q][t] * sb[s][t][i]) % PR;
                        nsb[nn][q][i] = acc;
                    }
                nn++;
                got += nd;
            }
            if (got != m) {
                nn = start;
                nsdim[nn] = m;
                for (t = 0; t < m; t++)
                    for (i = 0; i < nc; i++) nsb[nn][t][i] = sb[s][t][i];
                nn++;
            }
        }
        ns = nn;
        for (s = 0; s < ns; s++) {
            sdim[s] = nsdim[s];
            for (t = 0; t < sdim[s]; t++)
                for (i = 0; i < nc; i++) sb[s][t][i] = nsb[s][t][i];
        }
    }

    ndeg = 0;
    for (s = 0; s < ns; s++) {
        long long iv, S = 0, val, d;
        long long w[MC];
        if (sdim[s] != 1 || sb[s][0][0] == 0) continue;
        iv = minv(sb[s][0][0]);
        for (i = 0; i < nc; i++) w[i] = sb[s][0][i] * iv % PR;
        for (i = 0; i < nc; i++)
            S = (S + w[i] * w[cinv[i]] % PR * minv(csize[i] % PR)) % PR;
        if (S == 0) continue;
        val = (G % PR) * minv(S) % PR;
        for (d = 1; d <= PR / 2; d++)
            if (d * d % PR == val) { degs[ndeg++] = (int)d; break; }
    }
    for (i = 1; i < ndeg; i++) {
        int v = degs[i];
        k = i - 1;
        while (k >= 0 && degs[k] > v) { degs[k + 1] = degs[k]; k--; }
        degs[k + 1] = v;
    }
}

seq_t a(idx_t n) {
    if (!inited) init_all();
    if (n < 1 || n > (idx_t)ndeg) return (seq_t)0;
    return (seq_t)(long long)degs[n - 1];
}
