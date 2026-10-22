/*
 * A000160  Number of partitions into non-integral powers.
 * https://oeis.org/A000160
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{1<=x1<=x2<=x3<=x4 : x1^(2/3)+x2^(2/3)+x3^(2/3)+x4^(2/3)
 *            <= n}, counted by Burnside's lemma over S_4: a(n) = (T4 + 6*T211
 *            + 3*T22 + 8*T31 + 6*T4d)/24, where T4 counts ordered 4-tuples,
 *            T211 counts (a,b,c) with 2f(a)+f(b)+f(c)<=n, T22 counts (a,b)
 *            with 2f(a)+2f(b)<=n, T31 counts (a,b) with...
 * Range:     u8 n <= 10, u16 n <= 26, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   a160_f
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "../utils/a160_f.h"

typedef struct { double s; uint64_t w; } a160_pair;

static double *a160_F = 0;        /* a160_F[1..a160_M] */
static uint64_t a160_M = 0;
static a160_pair *a160_P = 0;     /* sorted by s, w = prefix weight */
static size_t a160_np = 0;
static uint64_t a160_cap = 0;

static int a160_cmp(const void *p, const void *q)
{
    double x = ((const a160_pair *)p)->s, y = ((const a160_pair *)q)->s;
    return (x > y) - (x < y);
}

static void a160_build(uint64_t c)
{
    double cd = (double)c;
    uint64_t M = (uint64_t)pow(cd, 1.5) + 2, x, a, b;
    size_t cnt = 0, k;
    free(a160_F);
    free(a160_P);
    a160_F = (double *)malloc((size_t)(M + 1) * sizeof(double));
    a160_F[0] = 0.0;
    for (x = 1; x <= M; x++) a160_F[x] = a160_f(x);
    for (a = 1; a <= M && 2.0 * a160_F[a] <= cd; a++)
        for (b = a; b <= M && a160_F[a] + a160_F[b] <= cd; b++) cnt++;
    a160_P = (a160_pair *)malloc((cnt ? cnt : 1) * sizeof(a160_pair));
    k = 0;
    for (a = 1; a <= M && 2.0 * a160_F[a] <= cd; a++)
        for (b = a; b <= M && a160_F[a] + a160_F[b] <= cd; b++) {
            a160_P[k].s = a160_F[a] + a160_F[b];
            a160_P[k].w = (a == b) ? 1 : 2;
            k++;
        }
    qsort(a160_P, cnt, sizeof(a160_pair), a160_cmp);
    for (k = 1; k < cnt; k++) a160_P[k].w += a160_P[k - 1].w;
    a160_np = cnt;
    a160_M = M;
    a160_cap = c;
}

/* number of pair entries with s <= R */
static size_t a160_upper(double R)
{
    size_t lo = 0, hi = a160_np;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (a160_P[mid].s <= R) lo = mid + 1; else hi = mid;
    }
    return lo;
}

static uint64_t a160_pw(size_t k) { return k ? a160_P[k - 1].w : 0; }

/* number of x >= 1 with f(x) <= R */
static uint64_t a160_cntF(double R)
{
    uint64_t lo = 1, hi = a160_M + 1;
    while (lo < hi) {
        uint64_t mid = lo + (hi - lo) / 2;
        if (a160_F[mid] <= R) lo = mid + 1; else hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    double nd = (double)n;
    unsigned __int128 T4 = 0, T211 = 0, T22, T31 = 0, T4d, tot;
    size_t i, j;
    uint64_t x;

    if (n < 4) return 0;
    if (n > a160_cap) {
        uint64_t c = (uint64_t)n;
        if (a160_cap && c < a160_cap + a160_cap / 8) c = a160_cap + a160_cap / 8;
        a160_build(c);
    }

    j = a160_upper(nd - 2.0);
    for (i = 0; i < a160_np && a160_P[i].s + 2.0 <= nd; i++) {
        uint64_t wi = a160_P[i].w - (i ? a160_P[i - 1].w : 0);
        while (j > 0 && a160_P[i].s + a160_P[j - 1].s > nd) j--;
        T4 += (unsigned __int128)wi * a160_pw(j);
    }
    for (x = 1; x <= a160_M && 2.0 * a160_F[x] + 2.0 <= nd; x++)
        T211 += a160_pw(a160_upper(nd - 2.0 * a160_F[x]));
    T22 = a160_pw(a160_upper(nd / 2.0));
    for (x = 1; x <= a160_M && 3.0 * a160_F[x] + 1.0 <= nd; x++)
        T31 += a160_cntF(nd - 3.0 * a160_F[x]);
    T4d = a160_cntF(nd / 4.0);

    tot = (T4 + 6 * T211 + 3 * T22 + 8 * T31 + 6 * T4d) / 24;

    {
        seq_t r = 0;
        int sh;
        for (sh = 120; sh >= 0; sh -= 8) {
            r = r * 256 + (seq_t)(unsigned)((tot >> sh) & 0xFF);
        }
        return r;
    }
}
