/*
 * A000818  Number of switching networks under action of GL_n(Z_2) acting on 3
 *          variables.
 * https://oeis.org/A000818
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/|GL_n(2)|) * Sum_{g in GL_n(2)} 8^{c(g)}, where c(g)
 *            is the number of cycles of g acting on the 2^n vectors of F_2^n
 *            (Burnside's lemma). The sum runs over conjugacy classes, using
 *            rational canonical form: each irreducible f != x gets a
 *            partition, the centralizer orders come from the sta...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   p2, pdeg, pmodp, pmulm, ppowx, pw8, ugcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/p2.h"
#include "../utils/pdeg.h"
#include "../utils/pmodp.h"
#include "../utils/pmulm.h"
#include "../utils/ppowx.h"
#include "../utils/pw8.h"
#include "../utils/ugcd.h"

static uint64_t *ipoly = NULL, *iord = NULL;
static int *ideg = NULL;
static int NIRR = 0, ICAP = 0, BUILT = 0;

static void build(int n) {
    int d;
    for (d = BUILT + 1; d <= n; d++) {
        uint64_t p, lo = ((uint64_t)1 << d) | 1, hi = (uint64_t)1 << (d + 1);
        for (p = lo; p < hi; p += 2) {
            int i, ok = 1;
            for (i = 0; i < NIRR && 2 * ideg[i] <= d; i++)
                if (pmodp(p, ipoly[i]) == 0) { ok = 0; break; }
            if (!ok) continue;
            {
                uint64_t N = ((uint64_t)1 << d) - 1, o = N, x = N, q;
                for (q = 2; q * q <= x; q++) {
                    if (x % q) continue;
                    while (x % q == 0) x /= q;
                    while (o % q == 0 && ppowx(o / q, p) == 1) o /= q;
                }
                if (x > 1) while (o % x == 0 && ppowx(o / x, p) == 1) o /= x;
                if (NIRR == ICAP) {
                    ICAP = ICAP ? 2 * ICAP : 64;
                    ipoly = (uint64_t *)realloc(ipoly, ICAP * sizeof(uint64_t));
                    iord = (uint64_t *)realloc(iord, ICAP * sizeof(uint64_t));
                    ideg = (int *)realloc(ideg, ICAP * sizeof(int));
                }
                ipoly[NIRR] = p; iord[NIRR] = o; ideg[NIRR] = d; NIRR++;
            }
        }
    }
    if (n > BUILT) BUILT = n;
}

static int CN = 0;
static int cdeg[128], ce[128];
static uint64_t cord[128];
static seq_t GLn, TOTAL;

static uint64_t cycles(void) {
    uint64_t m = 1, x, pr[64], divs[8192];
    int np = 0, nd = 1, i, j;
    unsigned __int128 sum = 0;
    for (i = 0; i < CN; i++) {
        uint64_t t = 1;
        while (t < (uint64_t)ce[i]) t *= 2;
        x = cord[i] * t;
        m = m / ugcd(m, x) * x;
    }
    divs[0] = 1;
    x = m;
    {
        uint64_t p;
        for (p = 2; p * p <= x; p++) {
            if (x % p) continue;
            {
                int cnt = nd, e;
                uint64_t pw = 1;
                pr[np++] = p;
                e = 0;
                while (x % p == 0) { x /= p; e++; }
                for (j = 1; j <= e; j++) {
                    pw *= p;
                    for (i = 0; i < cnt; i++) divs[nd++] = divs[i] * pw;
                }
            }
        }
        if (x > 1) {
            int cnt = nd;
            pr[np++] = x;
            for (i = 0; i < cnt; i++) divs[nd++] = divs[i] * x;
        }
    }
    for (i = 0; i < nd; i++) {
        uint64_t d = divs[i], odd = d, q = m / d, ph = q, D = 0;
        int s = 0;
        while (!(odd & 1)) { odd >>= 1; s++; }
        for (j = 0; j < np; j++) if (q % pr[j] == 0) ph = ph / pr[j] * (pr[j] - 1);
        for (j = 0; j < CN; j++) {
            if (odd % cord[j] == 0) {
                uint64_t lim = (s >= 40) ? (uint64_t)ce[j] : ((uint64_t)1 << s);
                uint64_t mn = (uint64_t)ce[j] < lim ? (uint64_t)ce[j] : lim;
                D += (uint64_t)cdeg[j] * mn;
            }
        }
        sum += (unsigned __int128)ph << D;
    }
    return (uint64_t)(sum / m);
}

static void dfs(int start, int rem, seq_t cent);

static void genpart(int idx, int left, int maxp, int np, int rem_after, seq_t cent) {
    int p;
    if (left == 0) {
        int d = ideg[idx], i, j;
        long long E = 0;
        seq_t fac = 1, Q = p2(d);
        for (i = 0; i < np; i++)
            for (j = 0; j < np; j++)
                E += ce[CN + i] < ce[CN + j] ? ce[CN + i] : ce[CN + j];
        i = 0;
        while (i < np) {
            int mlt = 0, k;
            seq_t Qm;
            j = i;
            while (j < np && ce[CN + j] == ce[CN + i]) { j++; mlt++; }
            E -= (long long)mlt * mlt;
            Qm = p2((long long)d * mlt);
            for (k = 0; k < mlt; k++) fac *= Qm - p2((long long)d * k);
            i = j;
        }
        (void)Q;
        fac *= p2((long long)d * E);
        for (i = 0; i < np; i++) { cdeg[CN + i] = d; cord[CN + i] = iord[idx]; }
        CN += np;
        dfs(idx + 1, rem_after, cent * fac);
        CN -= np;
        return;
    }
    for (p = left < maxp ? left : maxp; p >= 1; p--) {
        ce[CN + np] = p;
        genpart(idx, left - p, p, np + 1, rem_after, cent);
    }
}

static void dfs(int start, int rem, seq_t cent) {
    int idx;
    if (rem == 0) {
        if (cent != 0) TOTAL += GLn / cent * pw8(cycles());
        return;
    }
    for (idx = start; idx < NIRR && ideg[idx] <= rem; idx++) {
        int d = ideg[idx], k;
        for (k = 1; k * d <= rem; k++) genpart(idx, k, k, 0, rem - k * d, cent);
    }
}

seq_t a(idx_t n) {
    int nn = (int)n, k;
    build(nn);
    GLn = 1;
    for (k = 0; k < nn; k++) GLn *= p2(nn) - p2(k);
    TOTAL = 0;
    CN = 0;
    dfs(0, nn, (seq_t)1);
    if (GLn == 0) return 0;
    return TOTAL / GLn;
}
