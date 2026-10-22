/*
 * A000234  Partitions into non-integral powers (see Comments for precise
 *          definition).
 * https://oeis.org/A000234
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of multisets {x1<=x2<=...} of positive integers
 *            with Sum xi^(2/3) <= n. Search: write x = c*k^3 with c cubefree,
 *            so x^(2/3) = k^2*c^(2/3); class c holding sum of squares S
 *            contributes S*c^(2/3) with weight A001156(S). Meet in the
 *            middle: a sorted table for small classes (including c...
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>

typedef struct { double v; uint64_t w; } a234_ent;

static long double *Wc;      /* c^(2/3) for cubefree c >= 2, increasing */
static uint64_t ncf, Ksm;
static long double Nn;
static uint64_t *Ps;         /* partitions into squares */
static a234_ent *L;
static size_t Ln;
static uint64_t cntc, cntlim, total;

static void cnt_rec(uint64_t i, long double s)
{
    uint64_t j, m;
    cntc += (uint64_t)(Nn - s) + 1;
    if (cntc > cntlim) return;
    for (j = i; j < Ksm; j++) {
        if (s + Wc[j] > Nn) break;
        for (m = 1;; m++) {
            long double t = s + (long double)m * Wc[j];
            if (t > Nn) break;
            cnt_rec(j + 1, t);
            if (cntc > cntlim) return;
        }
    }
}

static void gen_rec(uint64_t i, long double s, uint64_t wt)
{
    uint64_t j, m, f = (uint64_t)(Nn - s);
    for (m = 0; m <= f; m++) {
        L[Ln].v = (double)(s + (long double)m);
        L[Ln].w = wt * Ps[m];
        Ln++;
    }
    for (j = i; j < Ksm; j++) {
        if (s + Wc[j] > Nn) break;
        for (m = 1;; m++) {
            long double t = s + (long double)m * Wc[j];
            if (t > Nn) break;
            gen_rec(j + 1, t, wt * Ps[m]);
        }
    }
}

static void sift(size_t root, size_t cnt)
{
    while (2 * root + 1 < cnt) {
        size_t ch = 2 * root + 1;
        if (ch + 1 < cnt && L[ch + 1].v > L[ch].v) ch++;
        if (L[root].v < L[ch].v) {
            a234_ent tmp = L[root]; L[root] = L[ch]; L[ch] = tmp;
            root = ch;
        } else return;
    }
}

static void heap_sort(void)
{
    size_t i, e;
    if (Ln < 2) return;
    for (i = Ln / 2; i-- > 0;) sift(i, Ln);
    for (e = Ln - 1; e > 0; e--) {
        a234_ent tmp = L[0]; L[0] = L[e]; L[e] = tmp;
        sift(0, e);
    }
}

/* total weight of table entries with value <= r */
static uint64_t pref_upto(long double r)
{
    size_t lo = 0, hi = Ln;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if ((long double)L[mid].v <= r) lo = mid + 1; else hi = mid;
    }
    return lo ? L[lo - 1].w : 0;
}

static void big_rec(uint64_t i, long double s, uint64_t wt)
{
    uint64_t j, m;
    total += wt * pref_upto(Nn - s);
    for (j = i; j < ncf; j++) {
        if (s + Wc[j] > Nn) break;
        for (m = 1;; m++) {
            long double t = s + (long double)m * Wc[j];
            if (t > Nn) break;
            big_rec(j + 1, t, wt * Ps[m]);
        }
    }
}

seq_t a(idx_t n)
{
    uint64_t n3 = (uint64_t)n * n * n, Mx, c, k, m, lo, hi, p;
    const uint64_t CAP = (uint64_t)1 << 20;
    unsigned char *cf;
    size_t i;

    /* Mx = largest x with x^2 <= n^3 */
    Mx = (uint64_t)sqrtl((long double)n3);
    while (Mx > 0 && Mx * Mx > n3) Mx--;
    while ((Mx + 1) * (Mx + 1) <= n3) Mx++;
    Nn = (long double)n;

    /* partitions into squares, up to n */
    Ps = (uint64_t *)calloc((size_t)n + 1, sizeof(uint64_t));
    Ps[0] = 1;
    for (k = 1; k * k <= n; k++)
        for (m = k * k; m <= n; m++) Ps[m] += Ps[m - k * k];

    /* cubefree c in [2, Mx] */
    cf = (unsigned char *)calloc((size_t)Mx + 2, 1);
    for (p = 2; p * p * p <= Mx; p++)
        for (c = p * p * p; c <= Mx; c += p * p * p) cf[c] = 1;
    Wc = (long double *)malloc(((size_t)Mx + 2) * sizeof(long double));
    ncf = 0;
    for (c = 2; c <= Mx; c++)
        if (!cf[c]) Wc[ncf++] = cbrtl((long double)c * (long double)c);
    free(cf);

    /* choose number of small cubefree classes so the table stays <= CAP */
    lo = 0; hi = ncf;
    while (lo < hi) {
        uint64_t mid = lo + (hi - lo + 1) / 2;
        Ksm = mid; cntc = 0; cntlim = CAP;
        cnt_rec(0, 0.0L);
        if (cntc <= CAP) lo = mid; else hi = mid - 1;
    }
    Ksm = lo;
    cntc = 0; cntlim = UINT64_MAX;
    cnt_rec(0, 0.0L);

    L = (a234_ent *)malloc(((size_t)cntc + 1) * sizeof(a234_ent));
    Ln = 0;
    gen_rec(0, 0.0L, 1);
    heap_sort();
    for (i = 1; i < Ln; i++) L[i].w += L[i - 1].w;   /* in-place prefix sums */

    total = 0;
    big_rec(Ksm, 0.0L, 1);

    free(L); L = NULL;
    free(Wc); Wc = NULL;
    free(Ps); Ps = NULL;
    return (seq_t)(total - 1);   /* drop the empty multiset */
}
