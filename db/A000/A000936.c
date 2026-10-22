/*
 * A000936  Number of free planar polyenoids with n nodes and symmetry point
 *          group C_{2v}.
 * https://oeis.org/A000936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   search, not a closed form. The code enumerates lattice trees on
 *            the honeycomb lattice that lie in half-planes, a quadrant or a
 *            wedge, using Redelmeier's method over bonds. From those counts,
 *            a(n) = (T_A(n) + T_B(n) - 2*N_D2h(n) - 2*N_D3h(n)) / 2, where
 *            T_A, T_B = trees that are mirror-symmetric a...
 * Range:     u8 n <= 15, u16 n <= 20, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   hx_inreg
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../utils/hx_inreg.h"

static int G_, MM_, LC_;
static unsigned char *inT_, *mark_, *reg_;
static int *lists_;
static uint64_t *cntp_;

static int hx_nbrs(int y, int *z, int *ed)
{
    int gx = y / G_, gy = y % G_;
    z[0] = y - G_; ed[0] = 2 * (y - G_);
    z[1] = y + G_; ed[1] = 2 * y;
    if (((gx + gy) & 1) == 0) { z[2] = y + 1; ed[2] = 2 * y + 1; }
    else { z[2] = y - 1; ed[2] = 2 * (y - 1) + 1; }
    return 3;
}

static void hx_rec(int lev, int ulen, int nv)
{
    int *U = lists_ + (size_t)lev * LC_;
    int *V = lists_ + (size_t)(lev + 1) * LC_;
    int i, j;
    for (i = 0; i < ulen; i++) {
        int e = U[i], p = e >> 1, q = (e & 1) ? p + 1 : p + G_, y;
        if (!inT_[p]) y = p; else if (!inT_[q]) y = q; else continue;
        inT_[y] = 1;
        cntp_[nv + 1]++;
        if (nv + 1 < MM_) {
            int k = 0, base, z[3], ed[3];
            for (j = i + 1; j < ulen; j++) V[k++] = U[j];
            base = k;
            hx_nbrs(y, z, ed);
            for (j = 0; j < 3; j++)
                if (reg_[z[j]] && !mark_[ed[j]]) { mark_[ed[j]] = 1; V[k++] = ed[j]; }
            hx_rec(lev + 1, k, nv + 1);
            for (j = base; j < k; j++) mark_[V[j]] = 0;
        }
        inT_[y] = 0;
    }
}

static void hx_enumerate(int type, int M, uint64_t *out)
{
    int off = M + 2, gx, gy, len = 0, j, r, z[3], ed[3];
    size_t cells;
    G_ = 2 * M + 5;
    cells = (size_t)G_ * G_;
    MM_ = M;
    LC_ = 2 * M + 16;
    inT_ = (unsigned char *)calloc(cells, 1);
    mark_ = (unsigned char *)calloc(2 * cells, 1);
    reg_ = (unsigned char *)calloc(cells, 1);
    lists_ = (int *)malloc(sizeof(int) * (size_t)(M + 3) * LC_);
    cntp_ = out;
    for (j = 0; j <= M; j++) out[j] = 0;
    for (gx = 1; gx < G_ - 1; gx++)
        for (gy = 1; gy < G_ - 1; gy++)
            reg_[gx * G_ + gy] = (unsigned char)hx_inreg(type, gx - off, gy - off);
    r = off * G_ + off; /* (0,0) */
    if (type == 1) {
        int w = r + 1, t, s;
        inT_[r] = 1; inT_[w] = 1;
        mark_[2 * r + 1] = 1;
        if (M >= 2) out[2] = 1;
        for (t = 0; t < 2; t++) {
            s = t ? w : r;
            hx_nbrs(s, z, ed);
            for (j = 0; j < 3; j++)
                if (reg_[z[j]] && !mark_[ed[j]]) { mark_[ed[j]] = 1; lists_[len++] = ed[j]; }
        }
        if (M > 2) hx_rec(0, len, 2);
    } else {
        if (type == 4) r = r + 1; /* (0,1) */
        inT_[r] = 1;
        if (M >= 1) out[1] = 1;
        hx_nbrs(r, z, ed);
        for (j = 0; j < 3; j++)
            if (reg_[z[j]] && !mark_[ed[j]]) { mark_[ed[j]] = 1; lists_[len++] = ed[j]; }
        if (M > 1) hx_rec(0, len, 1);
    }
    free(inT_); free(mark_); free(reg_); free(lists_);
}

static uint64_t *hx_cache[5];
static int hx_cmax[5];

static uint64_t hx_get(int type, int m)
{
    if (m < 1) return 0;
    if (hx_cache[type] == NULL || m > hx_cmax[type]) {
        uint64_t *buf = (uint64_t *)malloc(sizeof(uint64_t) * (size_t)(m + 1));
        hx_enumerate(type, m, buf);
        free(hx_cache[type]);
        hx_cache[type] = buf;
        hx_cmax[type] = m;
    }
    return hx_cache[type][m];
}

seq_t a(idx_t n)
{
    uint64_t TA = 0, TB = 0, D2 = 0, D3 = 0, r;
    int nn;
    if (n < 1) return (seq_t)0;
    nn = (int)n;
    if (nn % 2 == 1) {
        TA = 2 * hx_get(0, (nn + 1) / 2);
    } else {
        TA = hx_get(1, (nn + 2) / 2);
        TB = hx_get(2, nn / 2);
    }
    if (nn % 4 == 2) D2 = hx_get(3, (nn + 2) / 4);
    if (nn == 1) D3 = 1;
    else if (nn % 6 == 4) D3 = hx_get(4, (nn + 2) / 6);
    r = (TA + TB - 2 * D2 - 2 * D3) / 2;
    return (seq_t)r;
}
