/*
 * A000821  Number of switching networks under action of AG_n(Z_2) acting on 3
 *          variables.
 * https://oeis.org/A000821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/|AGL_n(2)|) * Sum_{g in AGL_n(2)} 8^cyc(g), where
 *            cyc(g) is the number of cycles of the affine map g on F_2^n
 *            (Burnside's lemma). The conjugacy classes of GL_n(2) are
 *            enumerated by rational canonical form. For each class, the
 *            translation vectors b are grouped by the Jordan type of the e...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 4, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   bpow2, gcd_u64, is_big_mode, pdeg_of, pmod2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/bpow2.h"
#include "../utils/gcd_u64.h"
#include "../utils/is_big_mode.h"
#include "../utils/pdeg_of.h"
#include "../utils/pmod2.h"

#define MAXN 40

static int g_n, g_big;
static int np = 0, pcap = 0;
static uint64_t *pf = NULL;
static int *pdeg = NULL;
static uint64_t *pord = NULL;
static int cur_depth;
static int sel_p[MAXN], sel_len[MAXN], sel_parts[MAXN][MAXN + 2];
static seq_t gS, gP;
static unsigned __int128 fS, fP;

static void build(int n)
{
    int d, i;
    np = 0;
    for (d = 1; d <= n; d++) {
        uint64_t f;
        for (f = ((uint64_t)1 << d) | 1; f < ((uint64_t)1 << (d + 1)); f += 2) {
            int irr = 1;
            for (i = 0; i < np && 2 * pdeg[i] <= d; i++)
                if (pmod2(f, pf[i]) == 0) { irr = 0; break; }
            if (!irr) continue;
            if (np == pcap) {
                pcap = pcap ? 2 * pcap : 64;
                pf = (uint64_t *)realloc(pf, sizeof(uint64_t) * pcap);
                pdeg = (int *)realloc(pdeg, sizeof(int) * pcap);
                pord = (uint64_t *)realloc(pord, sizeof(uint64_t) * pcap);
            }
            {
                uint64_t x = 1, m = 0;
                do {
                    x <<= 1;
                    if ((x >> d) & 1) x ^= f;
                    m++;
                } while (x != 1);
                pf[np] = f; pdeg[np] = d; pord[np] = m; np++;
            }
        }
    }
}

/* number of cycles of the linear map whose (x+1)-primary part has partition tp */
static uint64_t cyc(const int *tp, int tl)
{
    int ne = 0, r, e, t, maxp = 0;
    uint64_t ordv[MAXN + 1], L = 1, Lt, j;
    int dg[MAXN + 1], pl[MAXN + 1], sm[MAXN + 1][8];
    const int *pp[MAXN + 1];
    unsigned __int128 sum = 0;
    for (r = 0; r < cur_depth; r++)
        if (sel_p[r] != 0) {
            ordv[ne] = pord[sel_p[r]]; dg[ne] = pdeg[sel_p[r]];
            pp[ne] = sel_parts[r]; pl[ne] = sel_len[r]; ne++;
        }
    if (tl > 0) { ordv[ne] = 1; dg[ne] = 1; pp[ne] = tp; pl[ne] = tl; ne++; }
    for (r = 0; r < ne; r++) {
        L = L / gcd_u64(L, ordv[r]) * ordv[r];
        if (pp[r][0] > maxp) maxp = pp[r][0];
    }
    e = 0;
    while ((1 << e) < maxp) e++;
    for (r = 0; r < ne; r++)
        for (t = 0; t <= e; t++) {
            int s = 0, i;
            for (i = 0; i < pl[r]; i++) s += pp[r][i] < (1 << t) ? pp[r][i] : (1 << t);
            sm[r][t] = s;
        }
    Lt = L << e;
    for (j = 1; j <= Lt; j++) {
        int v = 0, K = 0;
        uint64_t o = j;
        while (!(o & 1)) { o >>= 1; v++; }
        if (v > e) v = e;
        for (r = 0; r < ne; r++)
            if (o % ordv[r] == 0) K += dg[r] * sm[r][v];
        sum += (unsigned __int128)1 << K;
    }
    return (uint64_t)(sum / Lt);
}

static void process(void)
{
    int n = g_n, E = 0, ne = 0, ll = 0, r, i, k, s, maxl;
    int elist[2 * MAXN + 2], lam[MAXN + 2], lc[MAXN + 4], mu[MAXN + 2];
    uint64_t cA;
    for (r = 0; r < cur_depth; r++) {
        int d = pdeg[sel_p[r]], len = sel_len[r], *parts = sel_parts[r];
        int sc2 = 0, sm2 = 0;
        for (k = 1; k <= parts[0]; k++) {
            int c = 0;
            for (i = 0; i < len; i++) if (parts[i] >= k) c++;
            sc2 += c * c;
        }
        i = 0;
        while (i < len) {
            int jj = i, m;
            while (jj < len && parts[jj] == parts[i]) jj++;
            m = jj - i;
            sm2 += m * m;
            E += d * m * (m - 1) / 2;
            for (k = 1; k <= m; k++) elist[ne++] = d * k;
            i = jj;
        }
        E += d * (sc2 - sm2);
        if (sel_p[r] == 0) {
            for (i = 0; i < len; i++) lam[i] = parts[i];
            ll = len;
        }
    }
    maxl = ll ? lam[0] : 0;
    for (k = 0; k <= maxl + 2; k++) {
        int c = 0;
        for (i = 0; i < ll; i++) if (lam[i] >= k) c++;
        lc[k] = c;
    }
    cA = cyc(lam, ll);
    {
        seq_t Qb = 0;
        unsigned __int128 Qf = 0;
        if (g_big) {
            seq_t D = 1;
            for (i = 0; i < ne; i++) D *= (bpow2(elist[i]) - 1);
            Qb = gP / D;
        } else {
            unsigned __int128 D = 1;
            for (i = 0; i < ne; i++) D *= (((unsigned __int128)1 << elist[i]) - 1);
            Qf = fP / D;
        }
        for (s = 1; s <= maxl + 1; s++) {
            int lcs = lc[s], ml, x1, x2;
            uint64_t c;
            if (s > 1 && !(lcs < lc[s - 1])) continue;
            for (i = 0; i < ll; i++) mu[i] = lam[i];
            ml = ll;
            if (s == 1) mu[ml++] = 1;
            else {
                for (i = 0; i < ml; i++) if (mu[i] == s - 1) { mu[i]++; break; }
            }
            c = cyc(mu, ml) - cA;
            x1 = n * (n - 1) / 2 - E + n - lcs + 3 * (int)c;
            x2 = s > 1 ? n * (n - 1) / 2 - E + n - lc[s - 1] + 3 * (int)c : -1;
            if (g_big) {
                seq_t term = bpow2(x1);
                if (x2 >= 0) term -= bpow2(x2);
                gS += Qb * term;
            } else {
                unsigned __int128 term = (unsigned __int128)1 << x1;
                if (x2 >= 0) term -= (unsigned __int128)1 << x2;
                fS += Qf * term;
            }
        }
    }
}

static void rec(int idx, int rem);

static void rec_part(int i, int rem, int left, int maxp)
{
    int dpt = cur_depth - 1, p;
    if (left == 0) { rec(i + 1, rem); return; }
    for (p = left < maxp ? left : maxp; p >= 1; p--) {
        sel_parts[dpt][sel_len[dpt]++] = p;
        rec_part(i, rem, left - p, p);
        sel_len[dpt]--;
    }
}

static void rec(int idx, int rem)
{
    int i, k;
    if (rem == 0) { process(); return; }
    for (i = idx; i < np && pdeg[i] <= rem; i++)
        for (k = 1; k * pdeg[i] <= rem; k++) {
            sel_p[cur_depth] = i;
            sel_len[cur_depth] = 0;
            cur_depth++;
            rec_part(i, rem - k * pdeg[i], k, k);
            cur_depth--;
        }
}

seq_t a(idx_t n)
{
    int k, nn;
    if (n < 1) return 0;
    g_big = is_big_mode();
    if (!g_big && n > 5) return 0;   /* a(6) exceeds 128 bits */
    if (n > MAXN - 2) return 0;
    nn = (int)n;
    g_n = nn;
    build(nn);
    cur_depth = 0;
    if (g_big) {
        gP = 1;
        for (k = 1; k <= nn; k++) gP *= (bpow2(k) - 1);
        gS = 0;
        rec(0, nn);
        return gS / (gP * bpow2((long long)nn * (nn + 1) / 2));
    } else {
        unsigned __int128 v;
        seq_t res = 0;
        int i;
        fP = 1;
        for (k = 1; k <= nn; k++) fP *= (((unsigned __int128)1 << k) - 1);
        fS = 0;
        rec(0, nn);
        v = fS / (fP << (nn * (nn + 1) / 2));
        for (i = 15; i >= 0; i--)
            res = res * 256 + (seq_t)(unsigned)((v >> (8 * i)) & 255);
        return res;
    }
}
