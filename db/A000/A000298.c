/*
 * A000298  Number of partitions into non-integral powers.
 * https://oeis.org/A000298
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of nonempty sets {x_1 < x_2 < ...} of positive
 *            integers with Sum_i sqrt(x_i) <= n. Computed by a search: a
 *            depth-first enumeration of the parts > 20 (largest part first).
 *            The remaining small parts are counted by binary search in
 *            precomputed sorted tables of subset sums. These are th...
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 36, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#define K0 20
#define R0 12
#define M0 (R0 * R0)
#define NB (1L << 18)
#define BSCALE 4096.0L /* NB / 64 */

typedef struct { long double s; int m; } a298_rec;

static long double *a298_G = NULL;
static long a298_Gn = 0;
static long *a298_Gidx = NULL;
static long double *a298_T[M0 + 1];
static long a298_Tn[M0 + 1];
static long double *a298_W = NULL;
static long long a298_Wn = 0;
static a298_rec *a298_recs = NULL;
static long a298_nrec = 0, a298_caprec = 0;
static int a298_inited = 0;

static int a298_cmp(const void *p, const void *q)
{
    long double x = ((const a298_rec *)p)->s, y = ((const a298_rec *)q)->s;
    return (x > y) - (x < y);
}

static void a298_tdfs(int start, long double s, int mx)
{
    int i;
    if (a298_nrec == a298_caprec) {
        a298_caprec = a298_caprec ? 2 * a298_caprec : 1024;
        a298_recs = (a298_rec *)realloc(a298_recs, sizeof(a298_rec) * (size_t)a298_caprec);
    }
    a298_recs[a298_nrec].s = s;
    a298_recs[a298_nrec].m = mx;
    a298_nrec++;
    for (i = start; i <= M0; i++) {
        long double t = s + sqrtl((long double)i);
        if (t > (long double)R0) break;
        a298_tdfs(i + 1, t, i);
    }
}

static void a298_init(void)
{
    long double *tmp;
    long sz = 1, b, i;
    int k;
    a298_G = (long double *)malloc(sizeof(long double) * ((size_t)1 << K0));
    tmp = (long double *)malloc(sizeof(long double) * ((size_t)1 << K0));
    a298_G[0] = 0.0L;
    for (k = 1; k <= K0; k++) {
        long double wk = sqrtl((long double)k);
        long p = 0, q = 0, o = 0;
        long double *sw;
        while (p < sz || q < sz) {
            if (q >= sz || (p < sz && a298_G[p] <= a298_G[q] + wk))
                tmp[o++] = a298_G[p++];
            else
                tmp[o++] = a298_G[q++] + wk;
        }
        sw = a298_G; a298_G = tmp; tmp = sw;
        sz *= 2;
    }
    free(tmp);
    a298_Gn = sz;
    a298_Gidx = (long *)malloc(sizeof(long) * (size_t)(NB + 2));
    i = 0;
    for (b = 0; b <= NB + 1; b++) {
        long double edge = (long double)b / BSCALE;
        while (i < a298_Gn && a298_G[i] < edge) i++;
        a298_Gidx[b] = i;
    }
    a298_tdfs(1, 0.0L, 0);
    qsort(a298_recs, (size_t)a298_nrec, sizeof(a298_rec), a298_cmp);
    for (k = K0 + 1; k <= M0; k++) {
        long c = 0, j;
        for (j = 0; j < a298_nrec; j++)
            if (a298_recs[j].m <= k) c++;
        a298_T[k] = (long double *)malloc(sizeof(long double) * (size_t)(c ? c : 1));
        a298_Tn[k] = c;
        c = 0;
        for (j = 0; j < a298_nrec; j++)
            if (a298_recs[j].m <= k) a298_T[k][c++] = a298_recs[j].s;
    }
    a298_inited = 1;
}

/* number of subsets of {1..20} with sum <= r */
static uint64_t a298_gcount(long double r)
{
    long b, lo, hi;
    if (r < 0) return 0;
    if (r >= a298_G[a298_Gn - 1]) return (uint64_t)a298_Gn;
    b = (long)floorl(r * BSCALE);
    lo = a298_Gidx[b];
    hi = a298_Gidx[b + 1];
    while (lo < hi) {
        long mid = lo + (hi - lo) / 2;
        if (a298_G[mid] <= r) lo = mid + 1; else hi = mid;
    }
    return (uint64_t)lo;
}

/* number of subsets of {1..k} with sum <= r, for r <= R0 */
static uint64_t a298_tcount(long long k, long double r)
{
    const long double *t = a298_T[k];
    long lo = 0, hi = a298_Tn[k];
    while (lo < hi) {
        long mid = lo + (hi - lo) / 2;
        if (t[mid] <= r) lo = mid + 1; else hi = mid;
    }
    return (uint64_t)lo;
}

/* number of subsets (including empty) of {1..k} with sum of square roots <= r */
static uint64_t a298_h(long long k, long double r)
{
    long long kk, x;
    uint64_t s;
    if (r < 0) return 0;
    kk = (long long)(r * r);
    if (kk > k) kk = k;
    while (kk < k && a298_W[kk + 1] <= r) kk++;
    while (kk > 0 && a298_W[kk] > r) kk--;
    if (kk <= K0) return a298_gcount(r);
    if (r <= (long double)R0) return a298_tcount(kk, r);
    s = a298_gcount(r);
    for (x = K0 + 1; x <= kk; x++)
        s += a298_h(x - 1, r - a298_W[x]);
    return s;
}

seq_t a(idx_t n)
{
    long long N = (long long)n * (long long)n, i;
    if (!a298_inited) a298_init();
    if (N + 2 > a298_Wn) {
        long long cap = N + 2;
        a298_W = (long double *)realloc(a298_W, sizeof(long double) * (size_t)cap);
        for (i = a298_Wn; i < cap; i++) a298_W[i] = sqrtl((long double)i);
        a298_Wn = cap;
    }
    return (seq_t)(a298_h(N, (long double)n) - 1);
}
