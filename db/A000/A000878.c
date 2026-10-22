/*
 * A000878  Number of switching networks with GL(n,2) acting on the domain and
 *          GL(3,2) acting on the range.
 * https://oeis.org/A000878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Burnside's lemma over GL(n,2) x GL(3,2). a(n) =
 *            (1/(168*|GL(n,2)|)) * Sum_{g in GL(n,2)} Sum_{h in GL(3,2)}
 *            Prod_{cycles c of g on GF(2)^n} #Fix(h^{|c|}). The sum over g
 *            runs over conjugacy classes, which are given by rational
 *            canonical form (a partition for each irreducible polynomial f !=
 *            x). T...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   z878_addsh, z878_clog2, z878_deg, z878_divs, z878_gcd, z878_mod,
 *            z878_muls, z878_next_part, z878_shr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/z878_addsh.h"
#include "../utils/z878_clog2.h"
#include "../utils/z878_deg.h"
#include "../utils/z878_divs.h"
#include "../utils/z878_gcd.h"
#include "../utils/z878_mod.h"
#include "../utils/z878_muls.h"
#include "../utils/z878_next_part.h"
#include "../utils/z878_shr.h"

static int z878_N, z878_nP;
static uint64_t *z878_P, *z878_Pe;
static int *z878_Pd, *z878_cpi, *z878_clam, *z878_clen;
static uint32_t *z878_T, *z878_G, *z878_GC, *z878_tmp;
static size_t z878_LT, z878_LG;
static uint64_t *z878_sL, *z878_sC, *z878_nL, *z878_nC;
static int *z878_sT, *z878_nT;
static size_t z878_sN, z878_nN, z878_cap;

static void z878_ensure(size_t need) {
    if (need <= z878_cap) return;
    while (z878_cap < need) z878_cap = z878_cap ? z878_cap * 2 : 64;
    z878_sL = (uint64_t *)realloc(z878_sL, z878_cap * sizeof(uint64_t));
    z878_sC = (uint64_t *)realloc(z878_sC, z878_cap * sizeof(uint64_t));
    z878_nL = (uint64_t *)realloc(z878_nL, z878_cap * sizeof(uint64_t));
    z878_nC = (uint64_t *)realloc(z878_nC, z878_cap * sizeof(uint64_t));
    z878_sT = (int *)realloc(z878_sT, z878_cap * sizeof(int));
    z878_nT = (int *)realloc(z878_nT, z878_cap * sizeof(int));
}

static void z878_leaf(int depth) {
    static const uint32_t hsz[5] = {1, 21, 56, 42, 48};
    uint64_t Call = 0, C2 = 0, C3 = 0, C4 = 0, C7 = 0, E[5];
    int64_t A = 0;
    size_t s, u, Lx;
    int c, h;
    z878_ensure(1);
    z878_sN = 1; z878_sL[0] = 1; z878_sT[0] = 0; z878_sC[0] = 1;
    for (c = 0; c < depth; c++) {
        int pi = z878_cpi[c], d = z878_Pd[pi], len = z878_clen[c], j, i, prevS = 0;
        uint64_t e = z878_Pe[pi];
        int *lam = z878_clam + (size_t)c * (z878_N + 1);
        z878_nN = 0;
        for (j = 0; j <= lam[0]; j++) {
            int S = 0, tc = z878_clog2(j);
            uint64_t cnt, Lc = j ? e : 1;
            for (i = 0; i < len; i++) S += lam[i] < j ? lam[i] : j;
            cnt = j == 0 ? 1 : ((uint64_t)1 << (d * S)) - ((uint64_t)1 << (d * prevS));
            prevS = S;
            for (s = 0; s < z878_sN; s++) {
                uint64_t L = z878_sL[s] / z878_gcd(z878_sL[s], Lc) * Lc;
                int t = z878_sT[s] > tc ? z878_sT[s] : tc;
                uint64_t cc = z878_sC[s] * cnt;
                for (u = 0; u < z878_nN; u++) if (z878_nL[u] == L && z878_nT[u] == t) break;
                if (u == z878_nN) {
                    z878_ensure(z878_nN + 1);
                    z878_nL[u] = L; z878_nT[u] = t; z878_nC[u] = 0; z878_nN++;
                }
                z878_nC[u] += cc;
            }
        }
        { uint64_t *tp = z878_sL; z878_sL = z878_nL; z878_nL = tp; }
        { uint64_t *tp = z878_sC; z878_sC = z878_nC; z878_nC = tp; }
        { int *tp = z878_sT; z878_sT = z878_nT; z878_nT = tp; }
        z878_sN = z878_nN;
    }
    for (s = 0; s < z878_sN; s++) {
        uint64_t ell = z878_sL[s] << z878_sT[s], cyc = z878_sC[s] / ell;
        Call += cyc;
        if (z878_sT[s] >= 1) C2 += cyc;
        if (z878_sT[s] >= 2) C4 += cyc;
        if (z878_sL[s] % 3 == 0) C3 += cyc;
        if (z878_sL[s] % 7 == 0) C7 += cyc;
    }
    memcpy(z878_GC, z878_G, z878_LG * sizeof(uint32_t));
    for (c = 0; c < depth; c++) {
        int pi = z878_cpi[c], d = z878_Pd[pi], len = z878_clen[c], i, m, k;
        int *lam = z878_clam + (size_t)c * (z878_N + 1);
        int64_t sq = 0;
        for (i = 0; i < len; i++) sq += (int64_t)(2 * i + 1) * lam[i];
        i = 0;
        while (i < len) {
            m = 1;
            while (i + m < len && lam[i + m] == lam[i]) m++;
            sq -= (int64_t)m * (m + 1) / 2;
            for (k = 1; k <= m; k++) {
                uint64_t v = ((uint64_t)1 << (d * k)) - 1;
                if (v > 1) z878_divs(z878_GC, z878_LG, (uint32_t)v);
            }
            i += m;
        }
        A += (int64_t)d * sq;
    }
    z878_shr(z878_GC, z878_LG, (uint64_t)A);
    E[0] = 3 * Call; E[1] = 2 * Call + C2; E[2] = Call + 2 * C3;
    E[3] = Call + C2 + C4; E[4] = 3 * C7;
    for (h = 0; h < 5; h++) {
        memcpy(z878_tmp, z878_GC, z878_LG * sizeof(uint32_t));
        z878_muls(z878_tmp, z878_LG, hsz[h]);
        Lx = z878_LG;
        while (Lx > 0 && z878_tmp[Lx - 1] == 0) Lx--;
        if (Lx) z878_addsh(z878_T, z878_LT, z878_tmp, Lx, E[h]);
    }
}

static void z878_rec(int start, int rem, int depth) {
    int j, k;
    if (rem == 0) { z878_leaf(depth); return; }
    for (j = start; j < z878_nP && z878_Pd[j] <= rem; j++) {
        int d = z878_Pd[j];
        for (k = 1; k * d <= rem; k++) {
            int *p = z878_clam + (size_t)depth * (z878_N + 1);
            int len = 1;
            p[0] = k;
            do {
                z878_cpi[depth] = j; z878_clen[depth] = len;
                z878_rec(j + 1, rem - k * d, depth + 1);
            } while (z878_next_part(p, &len));
        }
    }
}

seq_t a(idx_t n) {
    int N = (int)n, d, k, i;
    size_t capP = 16, idx;
    uint64_t sh = (uint64_t)N * (N - 1) / 2;
    seq_t r = 0;
    z878_N = N;
    z878_nP = 0;
    z878_P = (uint64_t *)malloc(capP * sizeof(uint64_t));
    z878_Pe = (uint64_t *)malloc(capP * sizeof(uint64_t));
    z878_Pd = (int *)malloc(capP * sizeof(int));
    for (d = 1; d <= N; d++) {
        uint64_t f;
        for (f = ((uint64_t)1 << d) | 1; f < ((uint64_t)1 << (d + 1)); f += 2) {
            int ok = 1, q;
            for (q = 0; q < z878_nP && 2 * z878_Pd[q] <= d; q++)
                if (z878_mod(f, z878_P[q]) == 0) { ok = 0; break; }
            if (!ok) continue;
            if ((size_t)z878_nP == capP) {
                capP *= 2;
                z878_P = (uint64_t *)realloc(z878_P, capP * sizeof(uint64_t));
                z878_Pe = (uint64_t *)realloc(z878_Pe, capP * sizeof(uint64_t));
                z878_Pd = (int *)realloc(z878_Pd, capP * sizeof(int));
            }
            {
                uint64_t cur = 1, e = 0;
                do { cur <<= 1; if ((cur >> d) & 1) cur ^= f; e++; } while (cur != 1);
                z878_P[z878_nP] = f; z878_Pd[z878_nP] = d; z878_Pe[z878_nP] = e; z878_nP++;
            }
        }
    }
    z878_LG = (size_t)N * N / 32 + 4;
    z878_LT = (size_t)((3 * ((uint64_t)1 << N) + (uint64_t)N * N + 64) / 32) + 2;
    z878_G = (uint32_t *)calloc(z878_LG, sizeof(uint32_t));
    z878_GC = (uint32_t *)calloc(z878_LG, sizeof(uint32_t));
    z878_tmp = (uint32_t *)calloc(z878_LG, sizeof(uint32_t));
    z878_T = (uint32_t *)calloc(z878_LT, sizeof(uint32_t));
    z878_tmp[0] = 1;
    for (k = 1; k <= N; k++) z878_muls(z878_tmp, z878_LG, (uint32_t)(((uint64_t)1 << k) - 1));
    z878_addsh(z878_G, z878_LG, z878_tmp, z878_LG, sh);
    z878_cpi = (int *)malloc((size_t)(N + 1) * sizeof(int));
    z878_clen = (int *)malloc((size_t)(N + 1) * sizeof(int));
    z878_clam = (int *)malloc((size_t)(N + 1) * (N + 1) * sizeof(int));
    z878_rec(0, N, 0);
    z878_divs(z878_T, z878_LT, 168);
    for (k = 1; k <= N; k++) {
        uint64_t v = ((uint64_t)1 << k) - 1;
        if (v > 1) z878_divs(z878_T, z878_LT, (uint32_t)v);
    }
    z878_shr(z878_T, z878_LT, sh);
    idx = z878_LT;
    while (idx > 0 && z878_T[idx - 1] == 0) idx--;
    while (idx-- > 0) {
        for (i = 3; i >= 0; i--) {
            idx_t byte = (idx_t)((z878_T[idx] >> (8 * i)) & 255u);
            r = r * 256 + (seq_t)byte;
        }
    }
    free(z878_P); free(z878_Pe); free(z878_Pd);
    free(z878_G); free(z878_GC); free(z878_tmp); free(z878_T);
    free(z878_cpi); free(z878_clen); free(z878_clam);
    return r;
}
