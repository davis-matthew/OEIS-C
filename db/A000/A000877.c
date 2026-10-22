/*
 * A000877  Number of switching networks with GL(n,2) acting on the domain and
 *          GL(2,2) acting on the range.
 * https://oeis.org/A000877
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Burnside's lemma over GL(n,2) x GL(2,2) acting on functions f:
 *            GF(2)^n -> GF(2)^2. a(n) = (1/(6*|GL(n,2)|)) * Sum_{classes g}
 *            |class(g)| * (4^c(g) + 3*2^(c(g)+e(g)) + 2*4^t(g)), where c, e,
 *            t count the cycles of g on GF(2)^n that have any length, even
 *            length, or length divisible by 3. Conjugacy c...
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 4, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   x877_gcd, x877_irred, x877_mulm, x877_pdeg, x877_pgcd,
 *            x877_powx, x877_prem, x877_xmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/x877_gcd.h"
#include "../utils/x877_irred.h"
#include "../utils/x877_mulm.h"
#include "../utils/x877_pdeg.h"
#include "../utils/x877_pgcd.h"
#include "../utils/x877_powx.h"
#include "../utils/x877_prem.h"
#include "../utils/x877_xmod.h"

typedef unsigned __int128 x877_u128;
typedef struct { uint64_t len, cnt; } x877_ent;

static int X_np, X_cap, X_big, X_GLe;
static uint64_t *X_ord;
static int *X_deg;
static x877_u128 X_Sfix, X_GLodd;
static seq_t X_Sbig, X_GLbig;

static x877_u128 x877_p2(uint64_t e) { return e >= 128 ? (x877_u128)0 : ((x877_u128)1) << e; }
static x877_u128 x877_inv(x877_u128 q) {
    x877_u128 x = q; int i;
    for (i = 0; i < 7; i++) x *= (x877_u128)2 - q * x;
    return x;
}

static void x877_dfs(int start, int r, const x877_ent *dist, int dsz, int ce, x877_u128 codd, seq_t cbig);

static void x877_apply(int j, const int *parts, int np, int sum, int r, const x877_ent *dist, int dsz,
                       int ce, x877_u128 codd, seq_t cbig) {
    int d = X_deg[j], top = parts[0], k, i, a1, b1, csz = 0, nsz = 0, w = 0, sub = 0;
    uint64_t o = X_ord[j], prevN = 1;
    x877_ent comp[70];
    x877_ent *nd;
    x877_u128 od = codd;
    seq_t ob = cbig;
    comp[csz].len = 1; comp[csz].cnt = 1; csz++;
    for (k = 1; k <= top; k++) {
        int s = 0, t = 0;
        uint64_t N;
        for (i = 0; i < np; i++) s += parts[i] < k ? parts[i] : k;
        N = (uint64_t)1 << (d * s);
        while ((1 << t) < k) t++;
        comp[csz].len = o << t; comp[csz].cnt = N - prevN; csz++;
        prevN = N;
    }
    nd = (x877_ent *)malloc((size_t)dsz * csz * sizeof(x877_ent));
    for (a1 = 0; a1 < dsz; a1++)
        for (b1 = 0; b1 < csz; b1++) {
            uint64_t l1 = dist[a1].len, l2 = comp[b1].len;
            uint64_t L = l1 / x877_gcd(l1, l2) * l2, c = dist[a1].cnt * comp[b1].cnt;
            int f = -1;
            for (i = 0; i < nsz; i++) if (nd[i].len == L) { f = i; break; }
            if (f < 0) { nd[nsz].len = L; nd[nsz].cnt = c; nsz++; }
            else nd[f].cnt += c;
        }
    for (i = 0; i < np; i++) w += (2 * i + 1) * parts[i];
    i = 0;
    while (i < np) {
        int k2 = i, m, s;
        while (k2 < np && parts[k2] == parts[i]) k2++;
        m = k2 - i;
        sub += m * (m + 1) / 2;
        for (s = 1; s <= m; s++) {
            uint64_t v = ((uint64_t)1 << (d * s)) - 1;
            od *= v;
            if (X_big) ob = ob * (seq_t)v;
        }
        i = k2;
    }
    x877_dfs(j + 1, r - d * sum, nd, nsz, ce + d * (w - sub), od, ob);
    free(nd);
}

static void x877_parts(int j, int rem, int maxp, int *parts, int np, int sum, int r,
                       const x877_ent *dist, int dsz, int ce, x877_u128 codd, seq_t cbig) {
    int p;
    if (np > 0) x877_apply(j, parts, np, sum, r, dist, dsz, ce, codd, cbig);
    for (p = (maxp < rem ? maxp : rem); p >= 1; p--) {
        parts[np] = p;
        x877_parts(j, rem - p, p, parts, np + 1, sum + p, r, dist, dsz, ce, codd, cbig);
    }
}

static void x877_dfs(int start, int r, const x877_ent *dist, int dsz, int ce, x877_u128 codd, seq_t cbig) {
    int j;
    if (r == 0) {
        uint64_t c = 0, ev = 0, t3 = 0;
        int i;
        for (i = 0; i < dsz; i++) {
            uint64_t q = dist[i].cnt / dist[i].len;
            c += q;
            if (dist[i].len % 2 == 0) ev += q;
            if (dist[i].len % 3 == 0) t3 += q;
        }
        if (X_big) {
            seq_t one = 1;
            seq_t cs = X_GLbig / (cbig * (one << (idx_t)ce));
            seq_t F = (one << (idx_t)(2 * c)) + (one << (idx_t)(c + ev)) * 3 + (one << (idx_t)(2 * t3)) * 2;
            X_Sbig += cs * F;
        } else {
            x877_u128 cs = x877_p2((uint64_t)(X_GLe - ce)) * X_GLodd * x877_inv(codd);
            x877_u128 F = x877_p2(2 * c) + 3 * x877_p2(c + ev) + 2 * x877_p2(2 * t3);
            X_Sfix += cs * F;
        }
        return;
    }
    {
        int parts[70];
        for (j = start; j < X_np && X_deg[j] <= r; j++)
            x877_parts(j, r / X_deg[j], r / X_deg[j], parts, 0, 0, r, dist, dsz, ce, codd, cbig);
    }
}

seq_t a(idx_t n) {
    int N = (int)n, d, i;
    seq_t x = 1, one = 1;
    x877_ent st;
    for (i = 0; i < 130; i++) x += x;
    X_big = (x != 0);
    if (!X_big && n >= 7) return 0;
    X_np = 0; X_cap = 16;
    X_ord = (uint64_t *)malloc(X_cap * sizeof(uint64_t));
    X_deg = (int *)malloc(X_cap * sizeof(int));
    for (d = 1; d <= N; d++) {
        uint64_t M = ((uint64_t)1 << d) - 1, m = M, f;
        uint64_t pr[64]; int npr = 0;
        uint64_t p;
        for (p = 2; p * p <= m; p++)
            if (m % p == 0) { pr[npr++] = p; while (m % p == 0) m /= p; }
        if (m > 1) pr[npr++] = m;
        for (f = ((uint64_t)1 << d) | 1; f < ((uint64_t)1 << (d + 1)); f += 2) {
            uint64_t ord;
            if (!x877_irred(f, d)) continue;
            ord = M;
            for (i = 0; i < npr; i++)
                while (ord % pr[i] == 0 && x877_powx(ord / pr[i], f, d) == 1) ord /= pr[i];
            if (X_np == X_cap) {
                X_cap *= 2;
                X_ord = (uint64_t *)realloc(X_ord, X_cap * sizeof(uint64_t));
                X_deg = (int *)realloc(X_deg, X_cap * sizeof(int));
            }
            X_ord[X_np] = ord; X_deg[X_np] = d; X_np++;
        }
    }
    X_GLe = N * (N - 1) / 2;
    X_GLodd = 1;
    for (i = 1; i <= N; i++) X_GLodd *= (x877_u128)(((uint64_t)1 << i) - 1);
    X_Sfix = 0;
    if (X_big) {
        X_GLbig = one << (idx_t)X_GLe;
        for (i = 1; i <= N; i++) X_GLbig = X_GLbig * (seq_t)(((uint64_t)1 << i) - 1);
        X_Sbig = 0;
    }
    st.len = 1; st.cnt = 1;
    x877_dfs(0, N, &st, 1, 0, (x877_u128)1, one);
    free(X_ord); free(X_deg);
    if (X_big) return X_Sbig / (X_GLbig * 6);
    {
        x877_u128 v = (X_Sfix >> (1 + X_GLe)) * x877_inv((x877_u128)3 * X_GLodd);
        uint64_t lo = (uint64_t)v, hi = (uint64_t)(v >> 64);
        seq_t r = (seq_t)hi;
        seq_t s = (seq_t)(uint64_t)4294967296ULL;
        r = r * s;
        r = r * s;
        r += (seq_t)lo;
        return r;
    }
}
