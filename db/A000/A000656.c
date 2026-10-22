/*
 * A000656  Invertible Boolean functions of n variables with S(n) acting on
 *          the domain and GL(n,2) acting on the range.
 * https://oeis.org/A000656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Burnside's lemma over S(n) x GL(n,2). a(n) = (1/(n!*|GL(n,2)|))
 *            * Sum_{lambda |- n} (n!/z_lambda) * Sum_{B class of GL(n,2),
 *            same cycle type on F_2^n as the permutation matrix P_lambda}
 *            |class(B)| * Z(type), where Z(type) = Prod_d d^{m_d} * m_d!
 *            (centralizer of the cycle type in S(2^n)), m_d = (1...
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 4, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   igcd, mobius, pdegree, pmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "../utils/igcd.h"
#include "../utils/mobius.h"
#include "../utils/pdegree.h"
#include "../utils/pmod.h"

#define NMAX 15
#define KMAX 128
#define PMAX 6000
#define LMAX 200

typedef unsigned __int128 u128;

static int g_n;
static int npoly;
static uint32_t pl[PMAX];
static int pdeg[PMAX], pord[PMAX];

static int nlam;
static int lam_parts[LMAX][NMAX + 1], lam_np[LMAX], lam_L[LMAX];
static int lam_F[LMAX][KMAX + 1];
static uint64_t lam_coef[LMAX];
static u128 W128[LMAX];
static seq_t Wseq[LMAX];
static int kmax_used;

static int comp_poly[NMAX + 1], comp_parts[NMAX + 1][NMAX + 1], comp_np[NMAX + 1], ncomp;
static int dk[KMAX + 1];
static int cur[NMAX + 1];

static seq_t to_seq(u128 x)
{
    seq_t r = 0;
    int i;
    for (i = 3; i >= 0; i--) {
        r = r * 65536;
        r = r * 65536;
        r += (seq_t)(idx_t)((uint64_t)(x >> (32 * i)) & 0xffffffffu);
    }
    return r;
}

static void gen_polys(int n)
{
    int d, j;
    uint32_t p;
    npoly = 0;
    for (d = 1; d <= n; d++) {
        for (p = (1u << d) | 1u; p < (2u << d); p += 2) {
            int ok = 1;
            for (j = 0; j < npoly && 2 * pdeg[j] <= d; j++)
                if (pmod(p, pl[j], pdeg[j]) == 0) { ok = 0; break; }
            if (!ok) continue;
            {
                uint32_t c = pmod(2u, p, d);
                int e = 1;
                while (c != 1) {
                    c <<= 1;
                    if (c & (1u << d)) c ^= p;
                    e++;
                }
                pl[npoly] = p; pdeg[npoly] = d; pord[npoly] = e; npoly++;
            }
        }
    }
}

static void gen_lams(int rem, int maxp, int np)
{
    int p;
    if (rem == 0) {
        int i, k, L = 1, cnt[NMAX + 1];
        uint64_t z = 1, nf = 1;
        for (i = 0; i < np; i++) lam_parts[nlam][i] = cur[i];
        lam_np[nlam] = np;
        for (i = 0; i < np; i++) L = L / igcd(L, cur[i]) * cur[i];
        lam_L[nlam] = L;
        if (L > kmax_used) kmax_used = L;
        for (k = 1; k <= L; k++) {
            int c = 0;
            for (i = 0; i < np; i++) c += igcd(cur[i], k);
            lam_F[nlam][k] = c;
        }
        memset(cnt, 0, sizeof cnt);
        for (i = 0; i < np; i++) cnt[cur[i]]++;
        for (i = 1; i <= g_n; i++) {
            int t;
            for (t = 1; t <= cnt[i]; t++) z *= (uint64_t)i * (uint64_t)t;
        }
        for (i = 2; i <= g_n; i++) nf *= (uint64_t)i;
        lam_coef[nlam] = nf / z;
        W128[nlam] = 0;
        Wseq[nlam] = 0;
        nlam++;
        return;
    }
    for (p = (rem < maxp ? rem : maxp); p >= 1; p--) {
        cur[np] = p;
        gen_lams(rem - p, p, np + 1);
    }
}

static void eval_class(void)
{
    int k, l, c, i, any = 0;
    int match[LMAX];
    for (k = 1; k <= kmax_used; k++) {
        int a = 0, m = k, pa, s = 0;
        while (m % 2 == 0) { m /= 2; a++; }
        pa = 1 << a;
        for (c = 0; c < ncomp; c++) {
            int j = comp_poly[c], t = 0;
            if (m % pord[j]) continue;
            for (i = 0; i < comp_np[c]; i++)
                t += comp_parts[c][i] < pa ? comp_parts[c][i] : pa;
            s += pdeg[j] * t;
        }
        dk[k] = s;
    }
    for (l = 0; l < nlam; l++) {
        match[l] = 1;
        for (k = 1; k <= lam_L[l]; k++)
            if (dk[k] != lam_F[l][k]) { match[l] = 0; break; }
        any |= match[l];
    }
    if (!any) return;
    {
        int n = g_n, E = n * (n - 1) / 2, j;
        u128 R = 1, size128;
        seq_t sz;
        for (j = 1; j <= n; j++) R *= (((u128)1) << j) - 1;
        for (c = 0; c < ncomp; c++) {
            int d = pdeg[comp_poly[c]], np = comp_np[c];
            int s1 = 0, s2 = 0, s3 = 0;
            for (i = 0; i < np; i++) s1 += (2 * i + 1) * comp_parts[c][i];
            i = 0;
            while (i < np) {
                int m = 0, p = comp_parts[c][i], t;
                while (i < np && comp_parts[c][i] == p) { m++; i++; }
                s2 += m * m;
                s3 += m * (m - 1) / 2;
                for (t = 1; t <= m; t++) R /= (((u128)1) << (d * t)) - 1;
            }
            E -= d * (s1 - s2 + s3);
        }
        size128 = R << E;
        sz = to_seq(R);
        for (j = 0; j < E; j++) sz *= 2;
        for (l = 0; l < nlam; l++)
            if (match[l]) { W128[l] += size128; Wseq[l] += sz; }
    }
}

static void rec(int i, int r);

static void parts_gen(int j, int r_after, int sleft, int maxp)
{
    int p, c = ncomp - 1;
    if (sleft == 0) { rec(j + 1, r_after); return; }
    for (p = (sleft < maxp ? sleft : maxp); p >= 1; p--) {
        comp_parts[c][comp_np[c]++] = p;
        parts_gen(j, r_after, sleft - p, p);
        comp_np[c]--;
    }
}

static void rec(int i, int r)
{
    int j, s;
    if (r == 0) { eval_class(); return; }
    for (j = i; j < npoly && pdeg[j] <= r; j++) {
        int d = pdeg[j];
        for (s = 1; d * s <= r; s++) {
            comp_poly[ncomp] = j;
            comp_np[ncomp] = 0;
            ncomp++;
            parts_gen(j, r - d * s, s, s);
            ncomp--;
        }
    }
}

/* cycle counts m_d of P_lambda on 2^n points, for d | L */
static void cyc_counts(int l, int64_t *md)
{
    int L = lam_L[l], d, e, i;
    for (d = 1; d <= L; d++) {
        int64_t s = 0;
        md[d] = 0;
        if (L % d) continue;
        for (e = 1; e <= d; e++) {
            int c = 0, mu;
            if (d % e) continue;
            mu = mobius(d / e);
            if (!mu) continue;
            for (i = 0; i < lam_np[l]; i++) c += igcd(lam_parts[l][i], e);
            s += mu * ((int64_t)1 << c);
        }
        md[d] = s / d;
    }
}

seq_t a(idx_t n)
{
    int l, d, j;
    int64_t md[KMAX + 1];
    uint64_t nf = 1;
    if (n < 1 || n > NMAX) return 0;
    g_n = (int)n;
    gen_polys(g_n);
    nlam = 0;
    kmax_used = 1;
    gen_lams(g_n, g_n, 0);
    ncomp = 0;
    rec(0, g_n);
    for (j = 2; j <= g_n; j++) nf *= (uint64_t)j;

    if (n <= 5) {
        u128 tot = 0, gl = 1;
        for (l = 0; l < nlam; l++) {
            u128 Z = 1;
            int64_t t;
            if (!W128[l]) continue;
            cyc_counts(l, md);
            for (d = 1; d <= lam_L[l]; d++)
                for (t = 1; t <= md[d]; t++) Z *= (u128)d * (u128)t;
            tot += (u128)lam_coef[l] * W128[l] * Z;
        }
        for (j = 1; j <= g_n; j++) gl *= (((u128)1) << j) - 1;
        gl <<= g_n * (g_n - 1) / 2;
        return to_seq(tot / ((u128)nf * gl));
    } else {
        seq_t tot = 0, gl = 1;
        for (l = 0; l < nlam; l++) {
            seq_t Z = 1;
            int64_t t;
            if (!W128[l]) continue;
            cyc_counts(l, md);
            for (d = 1; d <= lam_L[l]; d++)
                for (t = 1; t <= md[d]; t++) {
                    Z *= (seq_t)(idx_t)d;
                    Z *= (seq_t)(idx_t)t;
                }
            tot += (seq_t)(idx_t)lam_coef[l] * Wseq[l] * Z;
        }
        for (j = 1; j <= g_n; j++) {
            seq_t p = 1;
            int i;
            for (i = 0; i < j; i++) p *= 2;
            gl *= p - 1;
        }
        for (j = 0; j < g_n * (g_n - 1) / 2; j++) gl *= 2;
        return tot / ((seq_t)(idx_t)nf * gl);
    }
}
