/*
 * A000820  Number of switching networks under action of AG_n(Z_2) acting on 2
 *          variables.
 * https://oeis.org/A000820
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/|AG_n(2)|) * Sum_{g in AG_n(2)} 4^cyc(g), i.e.
 *            Burnside count of orbits of AG_n(Z_2) on maps Z_2^n -> Z_2^2.
 *            The sum runs over GL_n(2) conjugacy classes (rational canonical
 *            forms) and over the translation types of the unipotent part
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   add_sh, bdiv, bmul, bshr, clmul, factor, ggcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/add_sh.h"
#include "../utils/bdiv.h"
#include "../utils/bmul.h"
#include "../utils/bshr.h"
#include "../utils/clmul.h"
#include "../utils/factor.h"
#include "../utils/ggcd.h"

#define MXN 24

static int nn;
static uint64_t *PL; static int *PD; static uint64_t *PO; static int NPL;
static int *PRT, *PLEN, *PK, *PSC2, *PMUL; static int NPR, CAPR;
static int PFIRST[MXN + 2];
static int CP[MXN], CQ[MXN], NC;
static uint32_t *G, *CS, *TMP, *TOT; static int WL; static size_t TL;

static uint64_t pmodp(uint64_t a, uint64_t f, int df) {
    int i;
    for (i = 63; i >= df; i--) if ((a >> i) & 1) a ^= f << (i - df);
    return a;
}

static uint64_t xpow(uint64_t e, uint64_t f, int df) {
    uint64_t r = 1, b = pmodp(2, f, df);
    while (e) {
        if (e & 1) r = pmodp(clmul(r, b), f, df);
        b = pmodp(clmul(b, b), f, df);
        e >>= 1;
    }
    return r;
}

static void pstore(const int *buf, int len, int k) {
    int idx, j, v;
    if (NPR == CAPR) {
        CAPR = CAPR ? 2 * CAPR : 64;
        PRT = (int *)realloc(PRT, sizeof(int) * CAPR * MXN);
        PLEN = (int *)realloc(PLEN, sizeof(int) * CAPR);
        PK = (int *)realloc(PK, sizeof(int) * CAPR);
        PSC2 = (int *)realloc(PSC2, sizeof(int) * CAPR);
        PMUL = (int *)realloc(PMUL, sizeof(int) * CAPR * (MXN + 1));
    }
    idx = NPR++;
    PLEN[idx] = len; PK[idx] = k;
    for (j = 0; j < len; j++) PRT[idx * MXN + j] = buf[j];
    for (v = 0; v <= MXN; v++) PMUL[idx * (MXN + 1) + v] = 0;
    for (j = 0; j < len; j++) PMUL[idx * (MXN + 1) + buf[j]]++;
    PSC2[idx] = 0;
    for (v = 1; len > 0 && v <= buf[0]; v++) {
        int c = 0;
        for (j = 0; j < len; j++) if (buf[j] >= v) c++;
        PSC2[idx] += c * c;
    }
}
static void pgen(int rem, int mx, int *buf, int len, int k) {
    int p;
    if (rem == 0) { pstore(buf, len, k); return; }
    for (p = rem < mx ? rem : mx; p >= 1; p--) { buf[len] = p; pgen(rem - p, p, buf, len + 1, k); }
}

static void process(void) {
    int n = nn, m = 0, mu = -1, c, E, J, nf, ell, j, t;
    long long tw = (long long)n * (n - 1) / 2;
    uint64_t Lodd = 1, L, pr[64]; int ex[64];
    uint64_t *dgo, *dph; int nd, cap;
    memcpy(CS, G, sizeof(uint32_t) * WL);
    for (c = 0; c < NC; c++) {
        int i = CP[c], p = CQ[c], d = PD[i], v;
        long long s = PSC2[p];
        if (i == 0) { mu = p; m = PK[p]; }
        else Lodd = Lodd / ggcd(Lodd, PO[i]) * PO[i];
        for (v = 1; v <= PK[p]; v++) {
            int mv = PMUL[p * (MXN + 1) + v];
            s -= (long long)mv * (mv + 1) / 2;
            for (j = 1; j <= mv; j++) bdiv(CS, (size_t)WL, (1ULL << (d * j)) - 1);
        }
        tw -= d * s;
    }
    E = 0; while ((1 << E) <= n) E++;
    L = Lodd << E;
    nf = factor(Lodd, pr, ex);
    cap = 1; for (j = 0; j < nf; j++) cap *= ex[j] + 1;
    dgo = (uint64_t *)malloc(sizeof(uint64_t) * cap);
    dph = (uint64_t *)malloc(sizeof(uint64_t) * cap);
    nd = 1; dgo[0] = 1;
    for (j = 0; j < nf; j++) {
        int cur = nd, a, q;
        uint64_t pw = 1;
        for (a = 1; a <= ex[j]; a++) {
            pw *= pr[j];
            for (q = 0; q < cur; q++) dgo[nd++] = dgo[q] * pw;
        }
    }
    for (j = 0; j < nd; j++) {
        uint64_t q = Lodd / dgo[j], ph = q;
        for (t = 0; t < nf; t++) if (q % pr[t] == 0) ph = ph / pr[t] * (pr[t] - 1);
        dph[j] = ph;
    }
    ell = mu >= 0 ? PLEN[mu] : 0;
    for (J = 0; J <= E; J++) {
        uint64_t odd, sh;
        unsigned __int128 sum = 0;
        uint64_t cyc;
        int e;
        if (J == 0) { odd = 1; sh = (uint64_t)(m - ell); }
        else {
            int a = m - ell, b = m - ell;
            for (t = 0; t < ell; t++) {
                int pp = PRT[mu * MXN + t];
                if (pp <= (1 << J) - 1) a++;
                if (pp <= (1 << (J - 1)) - 1) b++;
            }
            if (a == b) continue;
            odd = (1ULL << (a - b)) - 1; sh = (uint64_t)b;
        }
        for (j = 0; j < nd; j++)
            for (e = J; e <= E; e++) {
                int pe = 1 << e, D = 0;
                uint64_t ph = dph[j] * (e < E ? (1ULL << (E - e - 1)) : 1ULL);
                for (c = 0; c < NC; c++) {
                    int i = CP[c], p = CQ[c], s = 0;
                    if (i != 0 && dgo[j] % PO[i] != 0) continue;
                    for (t = 0; t < PLEN[p]; t++) {
                        int pp = PRT[p * MXN + t];
                        s += pp < pe ? pp : pe;
                    }
                    D += PD[i] * s;
                }
                sum += (unsigned __int128)ph << D;
            }
        cyc = (uint64_t)(sum / L);
        memcpy(TMP, CS, sizeof(uint32_t) * WL);
        bmul(TMP, WL, odd);
        add_sh(TOT, TMP, WL, 2 * cyc + (uint64_t)tw + (uint64_t)(n - m) + sh);
    }
    free(dgo); free(dph);
}

static void rec(int pi, int rem) {
    int i, k, p;
    if (rem == 0) { process(); return; }
    for (i = pi; i < NPL && PD[i] <= rem; i++) {
        int d = PD[i];
        for (k = 1; d * k <= rem; k++)
            for (p = PFIRST[k]; p < PFIRST[k + 1]; p++) {
                CP[NC] = i; CQ[NC] = p; NC++;
                rec(i + 1, rem - d * k);
                NC--;
            }
    }
}

seq_t a(idx_t n_)
{
    int n, d, k, i, cap;
    int buf[MXN + 1];
    seq_t r = 0;
    size_t q;
    if (n_ < 1 || n_ > 20) return 0;
    n = (int)n_; nn = n;

    cap = 16; NPL = 0;
    PL = (uint64_t *)malloc(sizeof(uint64_t) * cap);
    PD = (int *)malloc(sizeof(int) * cap);
    PO = (uint64_t *)malloc(sizeof(uint64_t) * cap);
    for (d = 1; d <= n; d++) {
        uint64_t f;
        for (f = (1ULL << d) | 1; f < (1ULL << (d + 1)); f += 2) {
            int ok = 1;
            for (i = 0; i < NPL && PD[i] <= d / 2; i++)
                if (pmodp(f, PL[i], PD[i]) == 0) { ok = 0; break; }
            if (!ok) continue;
            if (NPL == cap) {
                cap *= 2;
                PL = (uint64_t *)realloc(PL, sizeof(uint64_t) * cap);
                PD = (int *)realloc(PD, sizeof(int) * cap);
                PO = (uint64_t *)realloc(PO, sizeof(uint64_t) * cap);
            }
            {
                uint64_t N = (1ULL << d) - 1, o = N, pr[64]; int ex[64], nf, t;
                nf = factor(N, pr, ex);
                for (t = 0; t < nf; t++)
                    while (o % pr[t] == 0 && xpow(o / pr[t], f, d) == 1) o /= pr[t];
                PL[NPL] = f; PD[NPL] = d; PO[NPL] = o; NPL++;
            }
        }
    }

    NPR = 0; CAPR = 0; PRT = PLEN = PK = PSC2 = PMUL = NULL;
    for (k = 0; k <= n; k++) { PFIRST[k] = NPR; pgen(k, k, buf, 0, k); }
    PFIRST[n + 1] = NPR;

    WL = (n * (n + 1) / 2 + n) / 32 + 3;
    G = (uint32_t *)calloc((size_t)WL, sizeof(uint32_t));
    CS = (uint32_t *)calloc((size_t)WL, sizeof(uint32_t));
    TMP = (uint32_t *)calloc((size_t)WL, sizeof(uint32_t));
    G[0] = 1;
    for (i = 1; i <= n; i++) bmul(G, WL, (1ULL << i) - 1);
    TL = (((size_t)1 << (n + 1)) + (size_t)n * n + 3 * (size_t)n + 128) / 32 + 2 * (size_t)WL + 8;
    TOT = (uint32_t *)calloc(TL, sizeof(uint32_t));

    NC = 0;
    rec(0, n);

    bshr(TOT, TL, (uint64_t)n + (uint64_t)n * (n - 1) / 2);
    for (i = 2; i <= n; i++) bdiv(TOT, TL, (1ULL << i) - 1);

    for (q = TL; q-- > 0;) {
        int bb;
        for (bb = 3; bb >= 0; bb--)
            r = (seq_t)(r * 256 + (idx_t)((TOT[q] >> (8 * bb)) & 255));
    }

    free(PL); free(PD); free(PO);
    free(PRT); free(PLEN); free(PK); free(PSC2); free(PMUL);
    free(G); free(CS); free(TMP); free(TOT);
    return r;
}
