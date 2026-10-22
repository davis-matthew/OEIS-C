/*
 * A007846  There are three equivalent descriptions: 1. Number of
 *          (horizontally or vertically) connected arrays of 1..n on
 *          rectangular grid (otherwise zero) with only one local maximum. 2.
 *          Number of n-polyominoes labeled 1...n such that each successive
 *          labeled cell is the neighbor of a previously labeled cel...
 * https://oeis.org/A007846
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   no closed form; computed by search. Count labeled growth orders
 *            of fixed polyominoes: a level-by-level DP over free polyomino
 *            classes (sum of f over the fixed members of each class,
 *            propagated through canonical forms) up to size n-3, then a
 *            brute-force count of the last 3 growth steps for each class
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   decode, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/u128_to_seq.h"
#include "../utils/decode.h"

#define MAXN 24

typedef struct { uint64_t lo, hi; unsigned __int128 v; } tent;
typedef struct { tent *e; size_t cap, cnt; } ttab;

static int N, G, OFF;
static unsigned char *occ;
static unsigned int *mark;
static unsigned int stampc;
static int cells[MAXN + 4];
static int dirs[4];

static size_t tslot(const ttab *t, uint64_t lo, uint64_t hi) {
    uint64_t z = lo ^ (hi * 0x9E3779B97F4A7C15ULL);
    z ^= z >> 30; z *= 0xBF58476D1CE4E5B9ULL;
    z ^= z >> 27; z *= 0x94D049BB133111EBULL;
    z ^= z >> 31;
    return (size_t)z & (t->cap - 1);
}

static void tinit(ttab *t, size_t cap) {
    t->cap = cap; t->cnt = 0;
    t->e = (tent *)calloc(cap, sizeof(tent));
}

static void tadd(ttab *t, uint64_t lo, uint64_t hi, unsigned __int128 v);

static void tgrow(ttab *t) {
    tent *old = t->e;
    size_t ocap = t->cap, i;
    tinit(t, ocap * 2);
    for (i = 0; i < ocap; i++)
        if (old[i].hi) tadd(t, old[i].lo, old[i].hi, old[i].v);
    free(old);
}

static void tadd(ttab *t, uint64_t lo, uint64_t hi, unsigned __int128 v) {
    size_t s;
    if ((t->cnt + 1) * 4 > t->cap * 3) tgrow(t);
    s = tslot(t, lo, hi);
    while (t->e[s].hi && !(t->e[s].hi == hi && t->e[s].lo == lo)) s = (s + 1) & (t->cap - 1);
    if (!t->e[s].hi) { t->e[s].lo = lo; t->e[s].hi = hi; t->e[s].v = 0; t->cnt++; }
    t->e[s].v += v;
}

static void canon(const int *xs, const int *ys, int d, uint64_t *blo, uint64_t *bhi) {
    int tx[MAXN + 4], ty[MAXN + 4];
    int t, i;
    bool first = true;
    for (t = 0; t < 8; t++) {
        int minx = 1 << 30, miny = 1 << 30, maxx = -(1 << 30), w;
        uint64_t l = 0, h = 0;
        for (i = 0; i < d; i++) {
            int x = xs[i], y = ys[i];
            if (t & 1) x = -x;
            if (t & 2) y = -y;
            if (t & 4) { int s = x; x = y; y = s; }
            tx[i] = x; ty[i] = y;
            if (x < minx) minx = x;
            if (x > maxx) maxx = x;
            if (y < miny) miny = y;
        }
        w = maxx - minx + 1;
        for (i = 0; i < d; i++) {
            int b = (ty[i] - miny) * w + (tx[i] - minx);
            if (b < 64) l |= (uint64_t)1 << b;
            else h |= (uint64_t)1 << (b - 64);
        }
        h |= (uint64_t)w << 58;
        if (first || h < *bhi || (h == *bhi && l < *blo)) {
            *blo = l; *bhi = h; first = false;
        }
    }
}

static unsigned __int128 brute(int depth, int rem) {
    int P[4 * (MAXN + 4)];
    int k = 0, i, d;
    unsigned __int128 total = 0;
    if (rem == 0) return 1;
    stampc++;
    for (i = 0; i < depth; i++)
        for (d = 0; d < 4; d++) {
            int nb = cells[i] + dirs[d];
            if (!occ[nb] && mark[nb] != stampc) { mark[nb] = stampc; P[k++] = nb; }
        }
    if (rem == 1) return (unsigned __int128)k;
    if (rem == 2) {
        total = (unsigned __int128)k * (unsigned __int128)(k - 1);
        for (i = 0; i < k; i++)
            for (d = 0; d < 4; d++) {
                int nb = P[i] + dirs[d];
                if (!occ[nb] && mark[nb] != stampc) total++;
            }
        return total;
    }
    for (i = 0; i < k; i++) {
        occ[P[i]] = 1;
        cells[depth] = P[i];
        total += brute(depth + 1, rem - 1);
        occ[P[i]] = 0;
    }
    return total;
}

static unsigned __int128 res[MAXN + 1];
static bool have[MAXN + 1];

seq_t a(idx_t n) {
    int D, d, i, j, c, dd;
    int xs[MAXN + 4], ys[MAXN + 4];
    int P[4 * (MAXN + 4)];
    ttab cur, nxt;
    unsigned __int128 ans = 0;
    size_t s;

    if (n == 0) return (seq_t)1;
    if (n > MAXN) return (seq_t)0;
    if (have[n]) return u128_to_seq(res[n]);

    N = (int)n;
    D = N > 4 ? N - 3 : 1;
    G = 3 * N + 10;
    OFF = N + 4;
    dirs[0] = 1; dirs[1] = -1; dirs[2] = G; dirs[3] = -G;
    occ = (unsigned char *)calloc((size_t)G * G, 1);
    mark = (unsigned int *)calloc((size_t)G * G, sizeof(unsigned int));
    stampc = 0;

    tinit(&cur, 16);
    tadd(&cur, 1, (uint64_t)1 << 58, 1);

    for (d = 1; d < D; d++) {
        tinit(&nxt, 1024);
        for (s = 0; s < cur.cap; s++) {
            int k = 0;
            if (!cur.e[s].hi) continue;
            c = decode(cur.e[s].lo, cur.e[s].hi, xs, ys);
            for (i = 0; i < c; i++) {
                cells[i] = (xs[i] + OFF) + (ys[i] + OFF) * G;
                occ[cells[i]] = 1;
            }
            stampc++;
            for (i = 0; i < c; i++)
                for (dd = 0; dd < 4; dd++) {
                    int nb = cells[i] + dirs[dd];
                    if (!occ[nb] && mark[nb] != stampc) { mark[nb] = stampc; P[k++] = nb; }
                }
            for (j = 0; j < k; j++) {
                uint64_t lo = 0, hi = 0;
                xs[c] = P[j] % G - OFF;
                ys[c] = P[j] / G - OFF;
                canon(xs, ys, c + 1, &lo, &hi);
                tadd(&nxt, lo, hi, cur.e[s].v);
            }
            for (i = 0; i < c; i++) occ[cells[i]] = 0;
        }
        free(cur.e);
        cur = nxt;
    }

    for (s = 0; s < cur.cap; s++) {
        if (!cur.e[s].hi) continue;
        c = decode(cur.e[s].lo, cur.e[s].hi, xs, ys);
        for (i = 0; i < c; i++) {
            cells[i] = (xs[i] + OFF) + (ys[i] + OFF) * G;
            occ[cells[i]] = 1;
        }
        ans += cur.e[s].v * brute(c, N - c);
        for (i = 0; i < c; i++) occ[cells[i]] = 0;
    }

    free(cur.e);
    free(occ); free(mark);
    occ = 0; mark = 0;
    res[n] = ans;
    have[n] = true;
    return u128_to_seq(ans);
}
