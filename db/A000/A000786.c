/*
 * A000786  Number of inequivalent planar partitions of n, when considering
 *          them as 3D objects.
 * https://oeis.org/A000786
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (A000219(n) + 3*S(n) + 2*C(n))/6 (Burnside's lemma over
 *            the 6 permutations of the axes), where A000219 = plane
 *            partitions: g.f. Product_{k>=1} 1/(1-x^k)^k; S(n) = symmetric
 *            plane partitions: g.f. Product_{i>=1} 1/(1-x^(2i-1)) *
 *            Product_{m>=1} 1/(1-x^(2m))^floor(m/2); C(n) = cyclically
 *            symm...
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 52, u64 n <= 127,
 *            u128 n <= 150, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

typedef unsigned __int128 a786_u128;

static unsigned long long *cs_cnt = NULL;
static long long cs_N = -1;
static long long cs_lim = 0;
static int *cs_buf = NULL;
static long long cs_cap = 0;
static long long *cs_key = NULL;
static int *cs_wt = NULL, *cs_miss = NULL, *cs_up = NULL, *cs_nup = NULL;

static long long *cs_pstart = NULL, *cs_off = NULL;

static void cs_ensure(long long need)
{
    if (need > cs_cap) {
        long long nc = cs_cap * 2;
        if (nc < need) nc = need + 64;
        cs_buf = (int *)realloc(cs_buf, (size_t)nc * sizeof(int));
        cs_cap = nc;
    }
}

static long long cs_cid(long long i, long long j, long long k)
{
    long long N = cs_lim, p;
    if (i < 0 || j < 0 || k < 0) return -1;
    if ((i + 1) > N || (j + 1) > N / (i + 1)) return -1;
    p = (i + 1) * (j + 1);
    if ((k + 1) > N / p) return -1;
    return cs_off[cs_pstart[i] + j] + k;
}

static long long cs_rep(long long a, long long b, long long c)
{
    long long r[3][3];
    int best = 0, t, u;
    r[0][0] = a; r[0][1] = b; r[0][2] = c;
    r[1][0] = b; r[1][1] = c; r[1][2] = a;
    r[2][0] = c; r[2][1] = a; r[2][2] = b;
    for (t = 1; t < 3; t++) {
        for (u = 0; u < 3; u++) {
            if (r[t][u] < r[best][u]) { best = t; break; }
            if (r[t][u] > r[best][u]) break;
        }
    }
    return cs_cid(r[best][0], r[best][1], r[best][2]);
}

static void cs_dfs(long long lo, long long hi, long long w)
{
    long long t, s, nh, w2;
    int u, x, y;
    cs_cnt[w]++;
    for (t = lo; t < hi; t++) {
        x = cs_buf[t];
        w2 = w + cs_wt[x];
        if (w2 > cs_lim) continue;
        cs_ensure(hi + (hi - lo) + 4);
        nh = hi;
        for (s = lo; s < hi; s++) {
            y = cs_buf[s];
            if (cs_key[y] > cs_key[x] && w2 + cs_wt[y] <= cs_lim) cs_buf[nh++] = y;
        }
        for (u = 0; u < cs_nup[x]; u++) {
            y = cs_up[3 * x + u];
            if (--cs_miss[y] == 0 && w2 + cs_wt[y] <= cs_lim) cs_buf[nh++] = y;
        }
        cs_dfs(hi, nh, w2);
        for (u = 0; u < cs_nup[x]; u++) cs_miss[cs_up[3 * x + u]]++;
    }
}

static void cs_compute(long long N)
{
    long long i, j, k, P, nc, p, id, t;
    long long *ci, *cj, *ck;
    long long root;
    free(cs_cnt); free(cs_key); free(cs_wt); free(cs_miss); free(cs_up); free(cs_nup);
    free(cs_pstart); free(cs_off);
    cs_lim = N;
    cs_cnt = (unsigned long long *)calloc((size_t)(N + 1), sizeof(unsigned long long));
    cs_pstart = (long long *)malloc((size_t)(N + 1) * sizeof(long long));
    P = 0;
    for (i = 0; i < N; i++) { cs_pstart[i] = P; P += N / (i + 1); }
    cs_off = (long long *)malloc((size_t)(P + 1) * sizeof(long long));
    nc = 0;
    for (i = 0; i < N; i++)
        for (j = 0; j < N / (i + 1); j++) {
            cs_off[cs_pstart[i] + j] = nc;
            nc += N / ((i + 1) * (j + 1));
        }
    ci = (long long *)malloc((size_t)(nc + 1) * sizeof(long long));
    cj = (long long *)malloc((size_t)(nc + 1) * sizeof(long long));
    ck = (long long *)malloc((size_t)(nc + 1) * sizeof(long long));
    for (i = 0; i < N; i++)
        for (j = 0; j < N / (i + 1); j++)
            for (k = 0; k < N / ((i + 1) * (j + 1)); k++) {
                id = cs_off[cs_pstart[i] + j] + k;
                ci[id] = i; cj[id] = j; ck[id] = k;
            }
    cs_key = (long long *)malloc((size_t)(nc + 1) * sizeof(long long));
    cs_wt = (int *)malloc((size_t)(nc + 1) * sizeof(int));
    cs_miss = (int *)malloc((size_t)(nc + 1) * sizeof(int));
    cs_up = (int *)malloc((size_t)(3 * nc + 3) * sizeof(int));
    cs_nup = (int *)malloc((size_t)(nc + 1) * sizeof(int));
    cs_ensure(16);
    root = 0;
    for (id = 0; id < nc; id++) {
        long long a = ci[id], b = cj[id], c = ck[id];
        long long lowr[3], upr[3];
        int nl = 0, nu = 0, d, e, dup;
        cs_key[id] = (a + b + c) * nc + id;
        cs_nup[id] = 0;
        cs_miss[id] = 0;
        cs_wt[id] = (a == b && b == c) ? 1 : 3;
        if (cs_rep(a, b, c) != id) continue;
        for (d = 0; d < 3; d++) {
            long long da = a - (d == 0), db = b - (d == 1), dc = c - (d == 2);
            long long r;
            if (da < 0 || db < 0 || dc < 0) continue;
            r = cs_rep(da, db, dc);
            if (r < 0) continue;
            dup = 0;
            for (e = 0; e < nl; e++) if (lowr[e] == r) dup = 1;
            if (!dup) lowr[nl++] = r;
        }
        for (d = 0; d < 3; d++) {
            long long r = cs_rep(a + (d == 0), b + (d == 1), c + (d == 2));
            if (r < 0) continue;
            dup = 0;
            for (e = 0; e < nu; e++) if (upr[e] == r) dup = 1;
            if (!dup) upr[nu++] = r;
        }
        cs_miss[id] = nl;
        cs_nup[id] = nu;
        for (e = 0; e < nu; e++) cs_up[3 * id + e] = (int)upr[e];
    }
    for (id = 0; id < nc; id++) {
        if (cs_rep(ci[id], cj[id], ck[id]) == id && cs_miss[id] == 0 && cs_wt[id] <= N) {
            cs_ensure(root + 1);
            cs_buf[root++] = (int)id;
        }
    }
    cs_dfs(0, root, 0);
    free(ci); free(cj); free(ck);
    t = 0; (void)t; (void)p;
    cs_N = N;
}

static seq_t a786_to_seq(a786_u128 v)
{
    uint64_t hi = (uint64_t)(v >> 64), lo = (uint64_t)v;
    seq_t r;
    if (hi == 0) return (seq_t)lo;
    r = (seq_t)hi;
    r = r * (seq_t)65536; r = r * (seq_t)65536; r = r * (seq_t)65536; r = r * (seq_t)65536;
    r += (seq_t)lo;
    return r;
}

seq_t a(idx_t n)
{
    long long N = (long long)n, m, k, d, e;
    a786_u128 *pp, *sg, *sy, v, s;
    if (N > cs_N) cs_compute(N);
    pp = (a786_u128 *)malloc((size_t)(N + 1) * sizeof(a786_u128));
    sg = (a786_u128 *)malloc((size_t)(N + 1) * sizeof(a786_u128));
    sy = (a786_u128 *)malloc((size_t)(N + 1) * sizeof(a786_u128));
    for (m = 0; m <= N; m++) { sg[m] = 0; sy[m] = 0; }
    for (d = 1; d <= N; d++)
        for (m = d; m <= N; m += d) sg[m] += (a786_u128)d * (a786_u128)d;
    pp[0] = 1;
    for (m = 1; m <= N; m++) {
        s = 0;
        for (k = 1; k <= m; k++) s += sg[k] * pp[m - k];
        pp[m] = s / (a786_u128)m;
    }
    sy[0] = 1;
    for (d = 1; d <= N; d += 2)
        for (m = d; m <= N; m++) sy[m] += sy[m - d];
    for (k = 1; 2 * k <= N; k++) {
        d = 2 * k;
        for (e = 0; e < k / 2; e++)
            for (m = d; m <= N; m++) sy[m] += sy[m - d];
    }
    v = pp[N] + 3 * sy[N] + 2 * (a786_u128)cs_cnt[N];
    v /= 6;
    free(pp); free(sg); free(sy);
    return a786_to_seq(v);
}
