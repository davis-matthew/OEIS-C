/*
 * A006433  Number of tree-rooted planar maps with 4 vertices and n faces and
 *          no isthmuses.
 * https://oeis.org/A006433
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [y^(n-1)] A(y), where A = 5*D6 - (10*D5*D0 + 3*D3*D2 +
 *            2*D1*D4) + (6*D4*D0^2 + 2*D2^2*D0 + 6*D3*D1*D0 + D1^2*D2) -
 *            (D1^3*D0 + D3*D0^3 + 3*D2*D1*D0^2), D_k(y) = Sum_{m>=0}
 *            binomial(2m+k,k)*Catalan(m)*y^m (inclusion-exclusion over forced
 *            isthmuses of the spanning tree). With C = 1+w, y = w/(...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 23, u128 n <= 53,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

#define RFMAX 160
typedef struct { long long c[RFMAX]; int deg; int r; } rfun;

static void rf_reduce(rfun *f)
{
    while (f->r > 0 && f->deg >= 1) {
        long long s = 0;
        int i;
        for (i = 0; i <= f->deg; i++) s += f->c[i];
        if (s != 0) break;
        for (i = 1; i < f->deg; i++) f->c[i] += f->c[i - 1];
        f->c[f->deg] = 0;
        f->deg--;
        f->r--;
    }
}

static void rf_mul1mw(rfun *f)
{
    int i;
    for (i = f->deg + 1; i >= 1; i--) f->c[i] -= f->c[i - 1];
    f->deg++;
}

static void rf_raise(rfun *f, int R)
{
    while (f->r < R) { rf_mul1mw(f); f->r++; }
}

static void rf_mul(const rfun *a, const rfun *b, rfun *out)
{
    int i, j;
    memset(out, 0, sizeof(rfun));
    for (i = 0; i <= a->deg; i++)
        for (j = 0; j <= b->deg; j++)
            out->c[i + j] += a->c[i] * b->c[j];
    out->deg = a->deg + b->deg;
    out->r = a->r + b->r;
    rf_reduce(out);
}

static void next_D(const rfun *F, int k, rfun *G)
{
    long long dN[RFMAX], P[RFMAX], Q[RFMAX];
    int i, d = F->deg;
    memset(dN, 0, sizeof(dN));
    memset(P, 0, sizeof(P));
    memset(Q, 0, sizeof(Q));
    for (i = 0; i < d; i++) dN[i] = (long long)(i + 1) * F->c[i + 1];
    for (i = 0; i <= d; i++) {
        P[i] = dN[i] + (long long)F->r * F->c[i];
        if (i >= 1) P[i] -= dN[i - 1];
    }
    for (i = 0; i <= d + 2; i++) {
        long long v = 0;
        if (i >= 1) v += 2 * P[i - 1];
        if (i >= 2) v += 2 * P[i - 2];
        if (i <= d) v += (long long)k * F->c[i];
        if (i >= 1 && i - 1 <= d) v -= 2LL * k * F->c[i - 1];
        if (i >= 2 && i - 2 <= d) v += (long long)k * F->c[i - 2];
        Q[i] = v;
    }
    memset(G, 0, sizeof(rfun));
    for (i = 0; i <= d + 2; i++) G->c[i] = Q[i] / k;
    G->deg = d + 2;
    G->r = F->r + 2;
    rf_reduce(G);
}

static rfun total;
static int ready = 0;

static void setup(void)
{
    static rfun D[7];
    static const int coef[11] = { 5, -10, -3, -2, 6, 2, 6, 1, -1, -1, -3 };
    static const int idx[11][5] = {
        { 6, -1, -1, -1, -1 }, { 5, 0, -1, -1, -1 }, { 3, 2, -1, -1, -1 },
        { 1, 4, -1, -1, -1 }, { 4, 0, 0, -1, -1 }, { 2, 2, 0, -1, -1 },
        { 3, 1, 0, -1, -1 }, { 1, 1, 2, -1, -1 }, { 1, 1, 1, 0, -1 },
        { 3, 0, 0, 0, -1 }, { 2, 1, 0, 0, -1 }
    };
    int k, t, i, R;
    memset(&D[0], 0, sizeof(rfun));
    D[0].c[0] = 1; D[0].c[1] = 1; D[0].deg = 1; D[0].r = 0;
    for (k = 1; k <= 6; k++) next_D(&D[k - 1], k, &D[k]);
    memset(&total, 0, sizeof(rfun));
    for (t = 0; t < 11; t++) {
        rfun prod, tmp;
        prod = D[idx[t][0]];
        for (i = 1; i < 5 && idx[t][i] >= 0; i++) {
            rf_mul(&prod, &D[idx[t][i]], &tmp);
            prod = tmp;
        }
        R = prod.r > total.r ? prod.r : total.r;
        rf_raise(&prod, R);
        rf_raise(&total, R);
        if (prod.deg > total.deg) total.deg = prod.deg;
        for (i = 0; i <= prod.deg; i++) total.c[i] += (long long)coef[t] * prod.c[i];
        rf_reduce(&total);
    }
    while (total.deg > 0 && total.c[total.deg] == 0) total.deg--;
    rf_raise(&total, 1);
    ready = 1;
}

seq_t a(idx_t n)
{
    idx_t m, k, j;
    int i, p;
    seq_t *g, *hp, *hn, *b;
    seq_t sp = 0, sn = 0;
    if (n < 2) return 0;
    if (!ready) setup();
    m = n - 1;
    p = total.r - 1;
    g = alloc_zero(m + 1);
    hp = alloc_zero(m + 1);
    hn = alloc_zero(m + 1);
    b = alloc_zero(m + 1);
    g[0] = 1;
    for (k = 1; k <= m; k++) {
        if (p == 0) g[k] = 0;
        else g[k] = g[k - 1] * (seq_t)(k + (idx_t)p - 1) / (seq_t)k;
    }
    for (k = 0; k <= m; k++) {
        for (i = 0; i <= total.deg && (idx_t)i <= k; i++) {
            long long c = total.c[i];
            if (c > 0) hp[k] += g[k - i] * (seq_t)c;
            else if (c < 0) hn[k] += g[k - i] * (seq_t)(-c);
        }
    }
    b[0] = 1;
    for (j = 1; j <= m; j++) b[j] = b[j - 1] * (seq_t)(2 * m - j) / (seq_t)j;
    for (j = 0; j <= m; j++) {
        sp += b[j] * hp[m - j];
        sn += b[j] * hn[m - j];
    }
    release(g, m + 1);
    release(hp, m + 1);
    release(hn, m + 1);
    release(b, m + 1);
    return sp - sn;
}
