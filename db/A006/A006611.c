/*
 * A006611  Number of n-node forests not determined by their spectra.
 * https://oeis.org/A006611
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   no closed form; this is an exhaustive search. For a forest the
 *            characteristic polynomial is fixed by its matching numbers, so
 *            two forests are cospectral exactly when they have the same
 *            matching polynomial. Every free tree on <= n nodes is generated
 *            once, rooted at its centroid, from lists of root...
 * Range:     u8 n <= 11, u16 n <= 17, u32 n <= 19, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAXD 128

typedef struct {
    uint32_t *keys;
    uint64_t *vals; /* 0 marks an empty slot */
    size_t cap, cnt;
} fmap;

static int pd;   /* number of matching coefficients kept: n/2 + 1 */
static int gn;   /* current n */

static void fm_init(fmap *m)
{
    m->cap = 16;
    m->cnt = 0;
    m->keys = (uint32_t *)malloc(m->cap * (size_t)pd * sizeof(uint32_t));
    m->vals = (uint64_t *)calloc(m->cap, sizeof(uint64_t));
}

static void fm_free(fmap *m)
{
    free(m->keys);
    free(m->vals);
    m->keys = NULL;
    m->vals = NULL;
    m->cap = 0;
    m->cnt = 0;
}

static uint64_t fm_hash(const uint32_t *k)
{
    uint64_t h = 0x9E3779B97F4A7C15ULL;
    int i;
    for (i = 0; i < pd; i++) {
        h ^= k[i];
        h *= 0xff51afd7ed558ccdULL;
        h ^= h >> 31;
    }
    h ^= h >> 29;
    h *= 0xc4ceb9fe1a85ec53ULL;
    h ^= h >> 32;
    return h;
}

static void fm_put(fmap *m, const uint32_t *k, uint64_t v);

static void fm_grow(fmap *m)
{
    fmap g;
    size_t i;
    g.cap = m->cap * 2;
    g.cnt = 0;
    g.keys = (uint32_t *)malloc(g.cap * (size_t)pd * sizeof(uint32_t));
    g.vals = (uint64_t *)calloc(g.cap, sizeof(uint64_t));
    for (i = 0; i < m->cap; i++)
        if (m->vals[i])
            fm_put(&g, m->keys + i * (size_t)pd, m->vals[i]);
    fm_free(m);
    *m = g;
}

static void fm_put(fmap *m, const uint32_t *k, uint64_t v)
{
    size_t mask, i;
    if ((m->cnt + 1) * 4 > m->cap * 3)
        fm_grow(m);
    mask = m->cap - 1;
    i = (size_t)fm_hash(k) & mask;
    while (m->vals[i]) {
        if (memcmp(m->keys + i * (size_t)pd, k, (size_t)pd * sizeof(uint32_t)) == 0) {
            m->vals[i] += v;
            return;
        }
        i = (i + 1) & mask;
    }
    memcpy(m->keys + i * (size_t)pd, k, (size_t)pd * sizeof(uint32_t));
    m->vals[i] = v;
    m->cnt++;
}

static void pmul(uint32_t *o, const uint32_t *a, const uint32_t *b)
{
    uint32_t t[MAXD];
    int i, j;
    memset(t, 0, sizeof(t));
    for (i = 0; i < pd; i++)
        if (a[i])
            for (j = 0; i + j < pd; j++)
                t[i + j] += a[i] * b[j];
    memcpy(o, t, (size_t)pd * sizeof(uint32_t));
}

/* rooted trees: LA = matching poly of tree, LB = of tree minus root */
static uint32_t *LA, *LB;
static int *lsz;
static int lcnt, lcap;
static int *cumc;      /* cumc[z] = number of listed rooted trees of size <= z */
static uint32_t *stk;  /* per-depth (P, Q) buffers */
static fmap *U;

static void child(const uint32_t *P, const uint32_t *Q, int j, uint32_t *P2, uint32_t *Q2)
{
    uint32_t t1[MAXD], t2[MAXD];
    int k;
    const uint32_t *A = LA + (size_t)j * pd, *B = LB + (size_t)j * pd;
    pmul(t1, Q, A);
    pmul(t2, P, B);
    pmul(P2, P, A);
    for (k = 0; k < pd; k++)
        Q2[k] = t1[k] + t2[k];
}

static void addtree(const uint32_t *P, const uint32_t *Q, int s)
{
    int k;
    uint32_t *A, *B;
    if (lcnt == lcap) {
        lcap = lcap ? 2 * lcap : 64;
        LA = (uint32_t *)realloc(LA, (size_t)lcap * (size_t)pd * sizeof(uint32_t));
        LB = (uint32_t *)realloc(LB, (size_t)lcap * (size_t)pd * sizeof(uint32_t));
        lsz = (int *)realloc(lsz, (size_t)lcap * sizeof(int));
    }
    A = LA + (size_t)lcnt * pd;
    B = LB + (size_t)lcnt * pd;
    for (k = 0; k < pd; k++) {
        A[k] = P[k] + (k ? Q[k - 1] : 0);
        B[k] = P[k];
    }
    lsz[lcnt] = s;
    lcnt++;
}

/* build all rooted trees of size s: root + multiset of smaller rooted trees */
static void dfs_r(int maxi, int rem, int depth, int s)
{
    uint32_t *P = stk + (size_t)depth * 2 * pd, *Q = P + pd;
    uint32_t *P2 = P + 2 * pd, *Q2 = P2 + pd;
    int j, lim;
    if (rem == 0) {
        addtree(P, Q, s);
        return;
    }
    lim = cumc[rem] - 1;
    j = maxi < lim ? maxi : lim;
    for (; j >= 0; j--) {
        child(P, Q, j, P2, Q2);
        dfs_r(j, rem - lsz[j], depth + 1, s);
    }
}

/* trees rooted at their unique centroid: all branches of size < s/2 */
static void dfs_c(int maxi, int t, int f, int depth)
{
    uint32_t *P = stk + (size_t)depth * 2 * pd, *Q = P + pd;
    uint32_t *P2 = P + 2 * pd, *Q2 = P2 + pd;
    uint32_t M[MAXD];
    int j, k, rem, zmax, lim;
    if (t == 0 || 2 * f <= t) {
        for (k = 0; k < pd; k++)
            M[k] = P[k] + (k ? Q[k - 1] : 0);
        fm_put(&U[t + 1], M, 1);
    }
    rem = gn - 1 - t;
    zmax = (gn - 1) / 2;
    if (rem < zmax)
        zmax = rem;
    if (zmax < 1)
        return;
    lim = cumc[zmax] - 1;
    j = maxi < lim ? maxi : lim;
    for (; j >= 0; j--) {
        child(P, Q, j, P2, Q2);
        dfs_c(j, t + lsz[j], depth == 0 ? lsz[j] : f, depth + 1);
    }
}

static uint64_t multichoose(uint64_t u, int k)
{
    uint64_t r = 1;
    int i;
    for (i = 1; i <= k; i++)
        r = (uint64_t)((unsigned __int128)r * (u + (uint64_t)i - 1) / (uint64_t)i);
    return r;
}

static uint64_t compute(int n)
{
    int s, h, m, k, kmax, j;
    size_t i, i2;
    uint32_t key[MAXD];
    uint32_t *Ak;
    uint64_t *w;
    fmap *G;
    uint64_t res = 0;

    gn = n;
    pd = n / 2 + 1;
    LA = LB = NULL;
    lsz = NULL;
    lcnt = lcap = 0;
    cumc = (int *)calloc((size_t)(n + 2), sizeof(int));
    stk = (uint32_t *)calloc((size_t)(n + 2) * 2 * (size_t)pd, sizeof(uint32_t));
    stk[0] = 1;

    /* rooted trees up to size n/2 */
    cumc[0] = 0;
    for (s = 1; s <= n / 2; s++) {
        dfs_r(lcnt - 1, s - 1, 0, s);
        cumc[s] = lcnt;
    }

    U = (fmap *)malloc((size_t)(n + 1) * sizeof(fmap));
    for (s = 0; s <= n; s++)
        fm_init(&U[s]);
    dfs_c(lcnt - 1, 0, 0, 0);

    /* bicentroidal trees: two rooted trees of size s/2 joined at roots */
    for (s = 2; s <= n; s += 2) {
        int lo, hi, a, b;
        h = s / 2;
        lo = cumc[h - 1];
        hi = cumc[h];
        for (a = lo; a < hi; a++)
            for (b = a; b < hi; b++) {
                uint32_t t1[MAXD], t2[MAXD];
                pmul(t1, LA + (size_t)a * pd, LA + (size_t)b * pd);
                pmul(t2, LB + (size_t)a * pd, LB + (size_t)b * pd);
                for (j = 0; j < pd; j++)
                    key[j] = t1[j] + (j ? t2[j - 1] : 0);
                fm_put(&U[s], key, 1);
            }
    }
    free(LA);
    free(LB);
    free(lsz);
    free(cumc);
    free(stk);
    LA = LB = NULL;
    lsz = NULL;
    cumc = NULL;
    stk = NULL;

    /* forests: multisets of free trees, keyed by product of matching polys */
    Ak = (uint32_t *)malloc((size_t)(n + 1) * (size_t)pd * sizeof(uint32_t));
    w = (uint64_t *)malloc((size_t)(n + 1) * sizeof(uint64_t));
    G = (fmap *)malloc((size_t)(n + 1) * sizeof(fmap));
    for (m = 0; m <= n; m++)
        fm_init(&G[m]);
    memset(key, 0, sizeof(key));
    key[0] = 1;
    fm_put(&G[0], key, 1);
    for (s = 1; s <= n; s++) {
        kmax = n / s;
        for (i = 0; i < U[s].cap; i++) {
            const uint32_t *M;
            uint64_t u = U[s].vals[i];
            if (!u)
                continue;
            M = U[s].keys + i * (size_t)pd;
            memset(Ak, 0, (size_t)pd * sizeof(uint32_t));
            Ak[0] = 1;
            for (k = 1; k <= kmax; k++) {
                pmul(Ak + (size_t)k * pd, Ak + (size_t)(k - 1) * pd, M);
                w[k] = multichoose(u, k);
            }
            for (m = n; m >= s; m--)
                for (k = 1; k * s <= m; k++) {
                    fmap *sm = &G[m - k * s];
                    for (i2 = 0; i2 < sm->cap; i2++)
                        if (sm->vals[i2]) {
                            pmul(key, sm->keys + i2 * (size_t)pd, Ak + (size_t)k * pd);
                            fm_put(&G[m], key, sm->vals[i2] * w[k]);
                        }
                }
        }
        fm_free(&U[s]);
    }
    for (i = 0; i < G[n].cap; i++)
        if (G[n].vals[i] >= 2)
            res += G[n].vals[i];

    for (m = 0; m <= n; m++)
        fm_free(&G[m]);
    free(G);
    fm_free(&U[0]);
    free(U);
    U = NULL;
    free(Ak);
    free(w);
    return res;
}

seq_t a(idx_t n)
{
    static uint64_t memo[MAXD];
    static bool have[MAXD];
    uint64_t r;
    if (n < 1)
        return (seq_t)0;
    if (n < MAXD && have[n])
        return (seq_t)memo[n];
    r = compute((int)n);
    if (n < MAXD) {
        memo[n] = r;
        have[n] = true;
    }
    return (seq_t)r;
}
