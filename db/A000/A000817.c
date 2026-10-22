/*
 * A000817  Number of switching networks under action of GL_n(Z_2) acting on 2
 *          variables.
 * https://oeis.org/A000817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/|GL_n(2)|) * Sum_{g in GL_n(2)} 4^c(g), where c(g) is
 *            the number of cycles of g acting on F_2^n (Burnside: maps F_2^n
 *            -> F_2^2 up to GL_n(2) acting on inputs). Summed over conjugacy
 *            classes (rational canonical forms: a partition for each
 *            irreducible f != x), with class size |GL_n(2)|/|C...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   a817_addsh, a817_div, a817_gcd, a817_mul, a817_pmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a817_addsh.h"
#include "../utils/a817_div.h"
#include "../utils/a817_gcd.h"
#include "../utils/a817_mul.h"
#include "../utils/a817_pmod.h"

static int a817_n;
static uint64_t a817_K;
static int a817_np;
static int *a817_pdeg;
static uint64_t *a817_pord;
static int **a817_plist;
static int *a817_pcnt;
static int a817_cd[70];
static uint64_t a817_co[70];
static const int *a817_cp[70];
static int a817_nc;
static uint32_t *a817_T, *a817_G, *a817_tmp;
static size_t a817_LT, a817_LG;

static int *a817_gbuf;
static int a817_gpos, a817_gstride;
static int a817_cur[70];

static void a817_gen(int rem, int maxp, int len)
{
    int q, i;
    if (rem == 0) {
        int *p = a817_gbuf + (size_t)a817_gpos * a817_gstride;
        p[0] = len;
        for (i = 0; i < len; i++) p[1 + i] = a817_cur[i];
        a817_gpos++;
        return;
    }
    for (q = (rem < maxp ? rem : maxp); q >= 1; q--) {
        a817_cur[len] = q;
        a817_gen(rem - q, q, len + 1);
    }
}

static void a817_leaf(void)
{
    uint64_t m = 1, t, sum = 0, c, sh;
    long long e = 0;
    int ci, i, j, np = 0;
    uint64_t pr[64];
    int ex[64], cu[64];

    memcpy(a817_tmp, a817_G, a817_LG * sizeof(uint32_t));
    for (ci = 0; ci < a817_nc; ci++) {
        int d = a817_cd[ci];
        const int *p = a817_cp[ci];
        int len = p[0], l1 = p[1];
        uint64_t tt = 1, o;
        long long s2 = 0, sm = 0;
        while (tt < (uint64_t)l1) tt <<= 1;
        o = a817_co[ci] * tt;
        m = m / a817_gcd(m, o) * o;
        for (j = 1; j <= l1; j++) {
            long long cnt = 0;
            for (i = 0; i < len; i++) if (p[1 + i] >= j) cnt++;
            s2 += cnt * cnt;
        }
        i = 0;
        while (i < len) {
            int k, jj = i;
            long long mu;
            while (jj < len && p[1 + jj] == p[1 + i]) jj++;
            mu = jj - i;
            sm += mu * (mu + 1) / 2;
            for (k = 1; k <= mu; k++) {
                uint32_t f = (uint32_t)((1ULL << (d * k)) - 1);
                if (f > 1) a817_div(a817_tmp, a817_LG, f);
            }
            i = jj;
        }
        e += (long long)d * (s2 - sm);
    }

    t = m;
    {
        uint64_t p;
        for (p = 2; p * p <= t; p++)
            if (t % p == 0) {
                pr[np] = p; ex[np] = 0;
                while (t % p == 0) { t /= p; ex[np]++; }
                np++;
            }
        if (t > 1) { pr[np] = t; ex[np] = 1; np++; }
    }
    for (i = 0; i < np; i++) cu[i] = 0;
    for (;;) {
        uint64_t s = 1, ph = 1, so, pw;
        int v = 0, D = 0;
        for (i = 0; i < np; i++) {
            int b = ex[i] - cu[i];
            for (j = 0; j < cu[i]; j++) s *= pr[i];
            if (b > 0) {
                ph *= pr[i] - 1;
                for (j = 0; j < b - 1; j++) ph *= pr[i];
            }
        }
        so = s;
        while (so % 2 == 0) { so /= 2; v++; }
        pw = 1ULL << v;
        for (ci = 0; ci < a817_nc; ci++) {
            if (so % a817_co[ci] == 0) {
                const int *p = a817_cp[ci];
                uint64_t smin = 0;
                for (j = 0; j < p[0]; j++)
                    smin += ((uint64_t)p[1 + j] < pw) ? (uint64_t)p[1 + j] : pw;
                D += a817_cd[ci] * (int)smin;
            }
        }
        sum += ph << D;
        i = 0;
        while (i < np && cu[i] == ex[i]) { cu[i] = 0; i++; }
        if (i == np) break;
        cu[i]++;
    }
    c = sum / m;
    sh = 2 * c + a817_K - (uint64_t)e;
    a817_addsh(a817_T, a817_LT, a817_tmp, a817_LG, sh);
}

static void a817_rec(int i, int rem)
{
    if (rem == 0) { a817_leaf(); return; }
    for (; i < a817_np && a817_pdeg[i] <= rem; i++) {
        int d = a817_pdeg[i], k, j;
        for (k = 1; k * d <= rem; k++) {
            for (j = 0; j < a817_pcnt[k]; j++) {
                a817_cd[a817_nc] = d;
                a817_co[a817_nc] = a817_pord[i];
                a817_cp[a817_nc] = a817_plist[k] + (size_t)j * (k + 1);
                a817_nc++;
                a817_rec(i + 1, rem - k * d);
                a817_nc--;
            }
        }
    }
}

seq_t a(idx_t n)
{
    int N = (int)n, d, k, i;
    uint64_t *polys;
    size_t cap;
    long long *pc;
    seq_t r = 0;

    a817_n = N;
    a817_K = (uint64_t)N * (uint64_t)(N - 1) / 2;

    /* irreducible polynomials over F_2 of degree <= N, excluding x, with orders */
    cap = ((size_t)1 << N) + 4;
    polys = (uint64_t *)malloc(cap * sizeof(uint64_t));
    a817_pdeg = (int *)malloc(cap * sizeof(int));
    a817_pord = (uint64_t *)malloc(cap * sizeof(uint64_t));
    a817_np = 0;
    for (d = 1; d <= N; d++) {
        uint64_t f;
        for (f = (1ULL << d) | 1ULL; f < (2ULL << d); f += 2) {
            int ok = 1, j;
            uint64_t y, e;
            for (j = 0; j < a817_np && 2 * a817_pdeg[j] <= d; j++)
                if (a817_pmod(f, polys[j], a817_pdeg[j]) == 0) { ok = 0; break; }
            if (!ok) continue;
            y = a817_pmod(2, f, d);
            e = 1;
            while (y != 1) { y = a817_pmod(y << 1, f, d); e++; }
            polys[a817_np] = f;
            a817_pdeg[a817_np] = d;
            a817_pord[a817_np] = e;
            a817_np++;
        }
    }

    /* partitions of k for k <= N */
    pc = (long long *)malloc((size_t)(N + 1) * sizeof(long long));
    for (k = 0; k <= N; k++) pc[k] = 0;
    pc[0] = 1;
    for (i = 1; i <= N; i++)
        for (k = i; k <= N; k++) pc[k] += pc[k - i];
    a817_pcnt = (int *)malloc((size_t)(N + 1) * sizeof(int));
    a817_plist = (int **)malloc((size_t)(N + 1) * sizeof(int *));
    for (k = 1; k <= N; k++) {
        a817_pcnt[k] = (int)pc[k];
        a817_plist[k] = (int *)malloc((size_t)pc[k] * (size_t)(k + 1) * sizeof(int));
        a817_gbuf = a817_plist[k];
        a817_gpos = 0;
        a817_gstride = k + 1;
        a817_gen(k, k, 0);
    }
    a817_pcnt[0] = 1;
    a817_plist[0] = NULL;

    /* big integers */
    a817_LG = (size_t)(N * N) / 32 + 2;
    a817_LT = (((size_t)2 << N) + (size_t)N * N + 64) / 32 + 1;
    a817_G = (uint32_t *)calloc(a817_LG, sizeof(uint32_t));
    a817_tmp = (uint32_t *)calloc(a817_LG, sizeof(uint32_t));
    a817_T = (uint32_t *)calloc(a817_LT, sizeof(uint32_t));
    a817_G[0] = 1;
    for (k = 1; k <= N; k++) a817_mul(a817_G, a817_LG, (uint32_t)((1ULL << k) - 1));

    a817_nc = 0;
    a817_rec(0, N);

    /* T = |GL_n(2)| * a(n); divide by 2^K and the odd part */
    {
        size_t off = (size_t)(a817_K / 32), ii;
        unsigned b = (unsigned)(a817_K % 32);
        for (ii = 0; ii < a817_LT; ii++) {
            uint64_t lo = ii + off < a817_LT ? a817_T[ii + off] : 0;
            uint64_t hi = ii + off + 1 < a817_LT ? a817_T[ii + off + 1] : 0;
            a817_T[ii] = b ? (uint32_t)((lo >> b) | (hi << (32 - b))) : (uint32_t)lo;
        }
    }
    for (k = 2; k <= N; k++) a817_div(a817_T, a817_LT, (uint32_t)((1ULL << k) - 1));

    {
        size_t ii;
        for (ii = a817_LT; ii-- > 0;) {
            r <<= 16;
            r <<= 16;
            r += (idx_t)a817_T[ii];
        }
    }

    for (k = 1; k <= N; k++) free(a817_plist[k]);
    free(a817_plist);
    free(a817_pcnt);
    free(pc);
    free(polys);
    free(a817_pdeg);
    free(a817_pord);
    free(a817_G);
    free(a817_tmp);
    free(a817_T);
    return r;
}
