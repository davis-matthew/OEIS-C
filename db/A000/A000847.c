/*
 * A000847  Number of n-input 2-output switching networks under action of
 *          GL(n,2) on the inputs and complementing group C(2,2) on the
 *          outputs.
 * https://oeis.org/A000847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Burnside over conjugacy classes of GL(n,2) (rational canonical
 *            forms: a partition lambda_f for each irreducible f != x): a(n) =
 *            Sum_{classes} 4^(cyc(g)-1) / |C(g)|, where cyc(g) = number of
 *            cycles of g on F_2^n (complementing an output has no fixed
 *            points since 0 is always fixed), |C(g)| = Prod_f...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   s847_gcd, s847_inv, s847_mulmod, s847_pdeg, s847_pmod,
 *            s847_powx, s847_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/s847_gcd.h"
#include "../utils/s847_inv.h"
#include "../utils/s847_mulmod.h"
#include "../utils/s847_pdeg.h"
#include "../utils/s847_pmod.h"
#include "../utils/s847_powx.h"
#include "../utils/s847_width.h"

static int s847_np;
static uint64_t *s847_poly, *s847_ord;
static int *s847_deg;
static int s847_W;
static seq_t s847_S, s847_u;

static seq_t s847_p2(uint64_t e)
{
    if (s847_W && e >= (uint64_t)s847_W) return 0;
    return (seq_t)1 << (idx_t)e;
}

static void s847_setup(int n)
{
    int cap = 1024, d;
    s847_np = 0;
    s847_poly = (uint64_t *)malloc(sizeof(uint64_t) * cap);
    s847_ord = (uint64_t *)malloc(sizeof(uint64_t) * cap);
    s847_deg = (int *)malloc(sizeof(int) * cap);
    for (d = 1; d <= n; d++) {
        uint64_t M = (d >= 64) ? 0 : ((1ull << d) - 1), pr[64], t;
        int npr = 0;
        uint64_t f, p;
        t = M;
        for (p = 2; p * p <= t; p++)
            if (t % p == 0) { pr[npr++] = p; while (t % p == 0) t /= p; }
        if (t > 1) pr[npr++] = t;
        for (f = (1ull << d) | 1; f < (1ull << (d + 1)); f += 2) {
            int i, ok = 1;
            for (i = 0; i < s847_np && 2 * s847_deg[i] <= d; i++)
                if (s847_pmod(f, s847_poly[i]) == 0) { ok = 0; break; }
            if (!ok) continue;
            {
                uint64_t o = M;
                int j;
                for (j = 0; j < npr; j++)
                    while (o % pr[j] == 0 && s847_powx(o / pr[j], f, d) == 1) o /= pr[j];
                if (s847_np == cap) {
                    cap *= 2;
                    s847_poly = (uint64_t *)realloc(s847_poly, sizeof(uint64_t) * cap);
                    s847_ord = (uint64_t *)realloc(s847_ord, sizeof(uint64_t) * cap);
                    s847_deg = (int *)realloc(s847_deg, sizeof(int) * cap);
                }
                s847_poly[s847_np] = f;
                s847_ord[s847_np] = o;
                s847_deg[s847_np] = d;
                s847_np++;
            }
        }
    }
}

static void s847_rec(int pi, int rem, const uint64_t *L, const uint64_t *C, int m, uint64_t v2, seq_t P);

static void s847_final(const uint64_t *L, const uint64_t *C, int m, uint64_t v2, seq_t P)
{
    uint64_t cyc = 0, e;
    int i;
    for (i = 0; i < m; i++) cyc += C[i] / L[i];
    e = 2 * (cyc - 1) - v2;
    if (s847_W) {
        seq_t t = s847_p2(e);
        if (t != 0) s847_S += 1u * t * s847_inv(P);
    } else {
        s847_S += (s847_u / P) << (idx_t)e;
    }
}

static void s847_apply(int q, int newrem, const int *parts, int np,
                       const uint64_t *L, const uint64_t *C, int m, uint64_t v2, seq_t P)
{
    int d = s847_deg[q], lam1 = parts[0], nb = 0, e, i, j, k, nm = 0;
    uint64_t o = s847_ord[q], prevdim = 0, sumsq = 0, sub = 0;
    uint64_t *bl = (uint64_t *)malloc(sizeof(uint64_t) * (lam1 + 2));
    uint64_t *bc = (uint64_t *)malloc(sizeof(uint64_t) * (lam1 + 2));
    uint64_t *nl, *nc;
    bl[0] = 1; bc[0] = 1; nb = 1;
    for (e = 1; e <= lam1; e++) {
        uint64_t dim = 0, cnt, len;
        int t = 0;
        for (j = 0; j < np; j++) dim += (uint64_t)d * (uint64_t)(parts[j] < e ? parts[j] : e);
        cnt = (1ull << dim) - (1ull << prevdim);
        prevdim = dim;
        while ((1 << t) < e) t++;
        len = o << t;
        if (nb > 1 && bl[nb - 1] == len) bc[nb - 1] += cnt;
        else { bl[nb] = len; bc[nb] = cnt; nb++; }
    }
    for (i = 1; i <= lam1; i++) {
        uint64_t lp = 0, mi = 0;
        for (j = 0; j < np; j++) {
            if (parts[j] >= i) lp++;
            if (parts[j] == i) mi++;
        }
        sumsq += lp * lp;
        sub += mi * (mi + 1) / 2;
        for (k = 1; k <= (int)mi; k++)
            P = 1u * P * (seq_t)((1ull << (d * k)) - 1);
    }
    v2 += (uint64_t)d * (sumsq - sub);
    nl = (uint64_t *)malloc(sizeof(uint64_t) * (m * nb + 1));
    nc = (uint64_t *)malloc(sizeof(uint64_t) * (m * nb + 1));
    for (i = 0; i < m; i++)
        for (j = 0; j < nb; j++) {
            uint64_t l = L[i] / s847_gcd(L[i], bl[j]) * bl[j];
            uint64_t c = C[i] * bc[j];
            for (k = 0; k < nm; k++) if (nl[k] == l) break;
            if (k == nm) { nl[nm] = l; nc[nm] = c; nm++; }
            else nc[k] += c;
        }
    free(bl); free(bc);
    s847_rec(q + 1, newrem, nl, nc, nm, v2, P);
    free(nl); free(nc);
}

static void s847_gen(int q, int rem, int left, int maxp, int *parts, int k, int sum,
                     const uint64_t *L, const uint64_t *C, int m, uint64_t v2, seq_t P)
{
    int d = s847_deg[q], p;
    for (p = (maxp < left ? maxp : left); p >= 1; p--) {
        parts[k] = p;
        s847_apply(q, rem - d * (sum + p), parts, k + 1, L, C, m, v2, P);
        s847_gen(q, rem, left - p, p, parts, k + 1, sum + p, L, C, m, v2, P);
    }
}

static void s847_rec(int pi, int rem, const uint64_t *L, const uint64_t *C, int m, uint64_t v2, seq_t P)
{
    int q;
    int parts[64];
    if (rem == 0) { s847_final(L, C, m, v2, P); return; }
    for (q = pi; q < s847_np && s847_deg[q] <= rem; q++) {
        int left = rem / s847_deg[q];
        s847_gen(q, rem, left, left, parts, 0, 0, L, C, m, v2, P);
    }
}

seq_t a(idx_t n)
{
    uint64_t L0[1] = {1}, C0[1] = {1};
    int i;
    s847_W = s847_width();
    s847_setup((int)n);
    s847_S = 0;
    s847_u = 1;
    if (!s847_W)
        for (i = 1; i <= (int)n; i++) s847_u = s847_u * (seq_t)((1ull << i) - 1);
    s847_rec(0, (int)n, L0, C0, 1, 0, (seq_t)1);
    free(s847_poly); free(s847_ord); free(s847_deg);
    if (!s847_W) return s847_S / s847_u;
    return s847_S;
}
