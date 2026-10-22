/*
 * A000833  Number of switching networks (see Harrison reference for precise
 *          definition).
 * https://oeis.org/A000833
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Burnside count of orbits of GL_n(2) x S_3 on maps F_2^n ->
 *            F_2^3: a(n) = (1/(6|GL_n(2)|)) * Sum_{classes g} (|GL|/|C(g)|) *
 *            (8^c(g) + 3*4^c_odd(g)*8^c_even(g) + 2*2^c_n3(g)*8^c_3(g)),
 *            where c(g) is the number of cycles of g on F_2^n (c_odd/c_even:
 *            cycles of odd/even length, c_3/c_n3: lengths divi...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   z833_deg, z833_g, z833_irred, z833_mod, z833_mulm, z833_p2,
 *            z833_pgcd, z833_powm
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/z833_deg.h"
#include "../utils/z833_g.h"
#include "../utils/z833_irred.h"
#include "../utils/z833_mod.h"
#include "../utils/z833_mulm.h"
#include "../utils/z833_p2.h"
#include "../utils/z833_pgcd.h"
#include "../utils/z833_powm.h"

typedef struct { uint64_t L, N; } z833_od;

static int z833_np;
static int z833_pd[4096];
static uint64_t z833_po[4096];
static uint64_t z833_cf[256];
static int z833_ncf;
static uint64_t z833_cexp;
static int z833_fixed;
static unsigned __int128 z833_accF, z833_glF;
static seq_t z833_accB, z833_glB;

static void z833_eval(const z833_od *map, int msz) {
    uint64_t C = 0, codd = 0, c3 = 0, ceven, cn3, e1, e2, e3;
    int i;
    for (i = 0; i < msz; i++) {
        uint64_t c = map[i].N / map[i].L;
        C += c;
        if (map[i].L % 2 == 1) codd += c;
        if (map[i].L % 3 == 0) c3 += c;
    }
    ceven = C - codd;
    cn3 = C - c3;
    e1 = 3 * C;
    e2 = 2 * codd + 3 * ceven;
    e3 = cn3 + 3 * c3;
    if (z833_fixed) {
        unsigned __int128 cen = (unsigned __int128)1 << z833_cexp, w, s;
        for (i = 0; i < z833_ncf; i++) cen *= z833_cf[i];
        w = z833_glF / cen;
        s = ((unsigned __int128)1 << e1) + 3 * ((unsigned __int128)1 << e2)
            + 2 * ((unsigned __int128)1 << e3);
        z833_accF += w * s;
    } else {
        seq_t cen = z833_p2(z833_cexp), w, s;
        for (i = 0; i < z833_ncf; i++) cen *= (seq_t)z833_cf[i];
        w = z833_glB / cen;
        s = z833_p2(e1) + (seq_t)3 * z833_p2(e2) + (seq_t)2 * z833_p2(e3);
        z833_accB += w * s;
    }
}

static void z833_rec(int j, int rem, const z833_od *map, int msz);

static void z833_part(int k, int left, int maxp, int *parts, int len, int remAfter,
                      const z833_od *map, int msz) {
    if (left == 0) {
        int d = z833_pd[k], lam1 = parts[0], t, i, a, b, nsz = 0, oldncf = z833_ncf;
        uint64_t o = z833_po[k], oldexp = z833_cexp, sumsq = 0, msum = 0;
        uint64_t ek[65];
        z833_od prim[65];
        z833_od *nm;
        int np = 0;
        ek[0] = 0;
        for (t = 1; t <= lam1; t++) {
            uint64_t s = 0;
            for (i = 0; i < len; i++) s += (uint64_t)(parts[i] < t ? parts[i] : t);
            ek[t] = (uint64_t)d * s;
        }
        prim[np].L = 1; prim[np].N = 1; np++;
        for (t = 1; t <= lam1; t++) {
            uint64_t L = o, c = 1;
            while ((int)c < t) { c <<= 1; L <<= 1; }
            prim[np].L = L;
            prim[np].N = ((uint64_t)1 << ek[t]) - ((uint64_t)1 << ek[t - 1]);
            np++;
        }
        nm = (z833_od *)malloc(sizeof(z833_od) * (size_t)(msz * np));
        for (a = 0; a < msz; a++)
            for (b = 0; b < np; b++) {
                uint64_t L = map[a].L / z833_g(map[a].L, prim[b].L) * prim[b].L;
                uint64_t N = map[a].N * prim[b].N;
                for (i = 0; i < nsz; i++) if (nm[i].L == L) break;
                if (i == nsz) { nm[nsz].L = L; nm[nsz].N = N; nsz++; }
                else nm[i].N += N;
            }
        for (t = 1; t <= lam1; t++) {
            uint64_t lp = 0, m = 0;
            for (i = 0; i < len; i++) {
                if (parts[i] >= t) lp++;
                if (parts[i] == t) m++;
            }
            sumsq += lp * lp;
            msum += m * (m + 1) / 2;
            for (i = 1; i <= (int)m; i++)
                z833_cf[z833_ncf++] = ((uint64_t)1 << (d * i)) - 1;
        }
        z833_cexp += (uint64_t)d * (sumsq - msum);
        z833_rec(k + 1, remAfter, nm, nsz);
        z833_cexp = oldexp;
        z833_ncf = oldncf;
        free(nm);
        return;
    }
    {
        int p;
        for (p = (left < maxp ? left : maxp); p >= 1; p--) {
            parts[len] = p;
            z833_part(k, left - p, p, parts, len + 1, remAfter, map, msz);
        }
    }
}

static void z833_rec(int j, int rem, const z833_od *map, int msz) {
    int k, s;
    int parts[64];
    if (rem == 0) { z833_eval(map, msz); return; }
    for (k = j; k < z833_np && z833_pd[k] <= rem; k++)
        for (s = 1; s * z833_pd[k] <= rem; s++)
            z833_part(k, s, s, parts, 0, rem - s * z833_pd[k], map, msz);
}

seq_t a(idx_t n) {
    int N, d, i;
    seq_t z = 0;
    z833_od start;
    if (n < 1 || n > 20) return 0;
    N = (int)n;
    z -= 1;
    z833_fixed = (z > 0);
    if (z833_fixed && n > 5) return 0;

    z833_np = 0;
    for (d = 1; d <= N; d++) {
        int dp[8], ndp = 0, x = d, p;
        uint64_t e = ((uint64_t)1 << d) - 1, r, fp[64];
        int nfp = 0;
        uint64_t low;
        for (p = 2; p <= x; p++)
            if (x % p == 0) { dp[ndp++] = p; while (x % p == 0) x /= p; }
        r = e;
        for (low = 2; low * low <= r; low++)
            if (r % low == 0) { fp[nfp++] = low; while (r % low == 0) r /= low; }
        if (r > 1) fp[nfp++] = r;
        for (low = 1; low < ((uint64_t)1 << d); low += 2) {
            uint64_t f = ((uint64_t)1 << d) | low, o, xr;
            if (!z833_irred(f, d, dp, ndp)) continue;
            xr = z833_mod(2, f);
            o = e;
            for (i = 0; i < nfp; i++)
                while (o % fp[i] == 0 && z833_powm(xr, o / fp[i], f, d) == 1) o /= fp[i];
            z833_pd[z833_np] = d;
            z833_po[z833_np] = o;
            z833_np++;
        }
    }

    z833_ncf = 0;
    z833_cexp = 0;
    if (z833_fixed) {
        z833_glF = 1;
        for (i = 0; i < N; i++)
            z833_glF *= (unsigned __int128)(((uint64_t)1 << N) - ((uint64_t)1 << i));
        z833_accF = 0;
    } else {
        seq_t P = z833_p2((uint64_t)N);
        z833_glB = 1;
        for (i = 0; i < N; i++) z833_glB *= P - z833_p2((uint64_t)i);
        z833_accB = 0;
    }
    start.L = 1; start.N = 1;
    z833_rec(0, N, &start, 1);
    if (z833_fixed) return (seq_t)(z833_accF / (6 * z833_glF));
    return z833_accB / ((seq_t)6 * z833_glB);
}
