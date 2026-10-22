/*
 * A004394  Superabundant [or super-abundant] numbers: n such that sigma(n)/n
 *          > sigma(m)/m for all m < n, sigma(n) being A000203(n), the sum of
 *          the divisors of n.
 * https://oeis.org/A004394
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   search -- a(n) are the records of sigma(m)/m, i.e. the Pareto
 *            frontier of (m, sigma(m)/m). It is built one prime at a time in
 *            log space. A partial product that is beaten by a smaller one
 *            (equal or larger sigma/m) is dropped. Exponents are capped by
 *            q^k <= r^2, where r is the prime after the large...
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 54, u64 n <= 117,
 *            u128 n <= 254, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>

typedef struct { long double x, y; int32_t node; } sa_st;
typedef struct { int32_t parent; uint32_t p, e; } sa_nd;

static sa_nd *sa_nodes = NULL;
static size_t sa_nn = 0, sa_ncap = 0;
static int32_t *sa_terms = NULL;
static size_t sa_nterms = 0;
static long double sa_L = 0;

#define SA_EPS 1e-16L

static int32_t sa_mk_node(int32_t parent, uint32_t p, uint32_t e) {
    if (sa_nn == sa_ncap) {
        sa_ncap = sa_ncap ? 2 * sa_ncap : 4096;
        sa_nodes = (sa_nd *)realloc(sa_nodes, sa_ncap * sizeof(sa_nd));
    }
    sa_nodes[sa_nn].parent = parent;
    sa_nodes[sa_nn].p = p;
    sa_nodes[sa_nn].e = e;
    return (int32_t)sa_nn++;
}

static void sa_build(long double L) {
    size_t lim = (size_t)(4 * L) + 100, i, j, np = 0, nuse = 0, ns, k;
    char *comp = (char *)calloc(lim + 1, 1);
    uint32_t *pr;
    long double th = 0;
    uint64_t R, R2;
    sa_st *st;
    long double elp[64], g[64];
    size_t idx[64];

    for (i = 2; i * i <= lim; i++)
        if (!comp[i])
            for (j = i * i; j <= lim; j += i) comp[j] = 1;
    pr = (uint32_t *)malloc((lim + 1) * sizeof(uint32_t));
    for (i = 2; i <= lim; i++)
        if (!comp[i]) pr[np++] = (uint32_t)i;
    free(comp);

    /* any superabundant m <= e^L uses an initial run of primes with theta(P) <= L */
    while (nuse < np && th + logl((long double)pr[nuse]) <= L) {
        th += logl((long double)pr[nuse]);
        nuse++;
    }
    R = nuse < np ? pr[nuse] : pr[np - 1];
    R2 = R * R;

    sa_nn = 0;
    ns = 1;
    st = (sa_st *)malloc(sizeof(sa_st));
    st[0].x = 0; st[0].y = 0; st[0].node = -1;

    for (k = 0; k < nuse; k++) {
        uint64_t p = pr[k], pk = 1;
        int cap = 0, e;
        long double lp = logl((long double)p), base = log1pl(-1.0L / (long double)p);
        long double best = -1;
        size_t nn = 0;
        sa_st *nx;

        while (pk * p <= R2 && cap < 63) { pk *= p; cap++; }
        for (e = 0; e <= cap; e++) {
            elp[e] = e * lp;
            g[e] = e ? log1pl(-powl((long double)p, -(long double)(e + 1))) - base : 0;
            idx[e] = 0;
        }
        nx = (sa_st *)malloc(ns * (size_t)(cap + 1) * sizeof(sa_st));
        for (;;) {
            int be = -1;
            long double bx = 0, by = 0;
            for (e = 0; e <= cap; e++) {
                long double x, y;
                if (idx[e] >= ns) continue;
                x = st[idx[e]].x + elp[e];
                if (x > L) { idx[e] = ns; continue; }
                y = st[idx[e]].y + g[e];
                if (be < 0 || x < bx || (x == bx && y > by)) { be = e; bx = x; by = y; }
            }
            if (be < 0) break;
            if (by > best + SA_EPS) {
                nx[nn].x = bx;
                nx[nn].y = by;
                nx[nn].node = be ? sa_mk_node(st[idx[be]].node, (uint32_t)p, (uint32_t)be)
                                 : st[idx[be]].node;
                nn++;
                best = by;
            }
            idx[be]++;
        }
        free(st);
        st = nx;
        ns = nn;
    }
    free(pr);

    free(sa_terms);
    sa_terms = (int32_t *)malloc((ns ? ns : 1) * sizeof(int32_t));
    for (i = 0; i < ns; i++) sa_terms[i] = st[i].node;
    sa_nterms = ns;
    free(st);
    sa_L = L;
}

seq_t a(idx_t n) {
    seq_t r = 1;
    int32_t nd;
    while (sa_nterms < n) sa_build(sa_L == 0 ? 16.0L : 2 * sa_L);
    for (nd = sa_terms[n - 1]; nd >= 0; nd = sa_nodes[nd].parent) {
        uint32_t e;
        for (e = 0; e < sa_nodes[nd].e; e++) r *= (seq_t)sa_nodes[nd].p;
    }
    return r;
}
