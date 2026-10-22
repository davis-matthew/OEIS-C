/*
 * A007149  2-part of number of graphs on n nodes.
 * https://oeis.org/A007149
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A007814(A000088(n)), where n!*A000088(n) =
 *            Sum_{partitions p of n} (n!/z(p)) * 2^(e(p)), e(p) = Sum_k
 *            floor(p_k/2) + Sum_{i<k} gcd(p_i,p_k); the sum is evaluated
 *            modulo 2^K (partitions whose term is divisible by 2^K are
 *            pruned), K raised until the sum is nonzero mod 2^K
 * Range:     u8 n <= 97, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   a7149_gcd, a7149_popc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/a7149_gcd.h"
#include "../utils/a7149_popc.h"

#define A7149_MAXW 64
#define A7149_INF ((long long)1 << 50)

static int a7149_N, a7149_W;
static long long a7149_K, a7149_V2N;
static long long *a7149_L;
static int *a7149_gt;
static int *a7149_v2t;
static uint64_t *a7149_iod;
static int *a7149_parts, *a7149_mult;
static int a7149_np;
static uint64_t a7149_S[A7149_MAXW];

static void a7149_mul(uint64_t *r, const uint64_t *x, const uint64_t *y)
{
    uint64_t t[A7149_MAXW];
    int i, j, W = a7149_W;
    for (i = 0; i < W; i++) t[i] = 0;
    for (i = 0; i < W; i++) {
        unsigned __int128 c = 0;
        uint64_t xi = x[i];
        if (!xi) continue;
        for (j = 0; i + j < W; j++) {
            c += (unsigned __int128)xi * y[j] + t[i + j];
            t[i + j] = (uint64_t)c;
            c >>= 64;
        }
    }
    for (i = 0; i < W; i++) r[i] = t[i];
}

static void a7149_inv(uint64_t *y, uint64_t x)
{
    uint64_t xs[A7149_MAXW], t[A7149_MAXW];
    int i, W = a7149_W;
    long long bits = 3;
    for (i = 0; i < W; i++) { xs[i] = 0; y[i] = 0; }
    xs[0] = x;
    y[0] = x;
    while (bits < 64LL * W) {
        uint64_t c = 3;
        a7149_mul(t, xs, y);
        for (i = 0; i < W; i++) {
            uint64_t s = ~t[i] + c;
            c = (s < c) ? 1 : 0;
            t[i] = s;
        }
        a7149_mul(y, y, t);
        bits *= 2;
    }
}

static void a7149_addsh(const uint64_t *z, long long t)
{
    int W = a7149_W, ws = (int)(t / 64), bs = (int)(t % 64), i;
    uint64_t c = 0;
    for (i = ws; i < W; i++) {
        int k = i - ws;
        uint64_t v = z[k] << bs, s, s2, c1, c2;
        if (bs && k > 0) v |= z[k - 1] >> (64 - bs);
        s = a7149_S[i] + v;
        c1 = (s < v) ? 1 : 0;
        s2 = s + c;
        c2 = (s2 < c) ? 1 : 0;
        a7149_S[i] = s2;
        c = c1 | c2;
    }
}

static void a7149_rec(int r, int maxj, long long e, long long v2z,
                      const uint64_t *iz, long long cnt)
{
    int j, mj, i, m, md, W = a7149_W, N1 = a7149_N + 1;
    if (r == 0) {
        long long t = e + a7149_V2N - v2z;
        if (t < a7149_K) a7149_addsh(iz, t);
        return;
    }
    mj = maxj < r ? maxj : r;
    for (j = mj; j >= 1; j--) {
        long long sj = 0, e2 = e, v2 = v2z;
        uint64_t iz2[A7149_MAXW];
        for (i = 0; i < a7149_np; i++)
            sj += (long long)a7149_mult[i] * a7149_gt[a7149_parts[i] * N1 + j];
        for (i = 0; i < W; i++) iz2[i] = iz[i];
        md = 0;
        for (m = 1; m * j <= r; m++) {
            int r2 = r - m * j;
            e2 += j / 2 + (long long)(m - 1) * j + sj;
            v2 += a7149_v2t[j] + a7149_v2t[m];
            if (j == 1 && r2 > 0) continue;
            if (r2 == 0) {
                if (e2 + a7149_V2N - v2 >= a7149_K) continue;
            } else {
                int mj2 = (j - 1 < r2) ? j - 1 : r2;
                long long lb;
                if (mj2 <= 0) continue;
                lb = e2 + a7149_V2N - v2 + a7149_L[(long long)r2 * N1 + mj2]
                     + (cnt + m) * (long long)((r2 + mj2 - 1) / mj2);
                if (lb >= a7149_K) continue;
            }
            while (md < m) {
                md++;
                a7149_mul(iz2, iz2, a7149_iod + (size_t)j * W);
                a7149_mul(iz2, iz2, a7149_iod + (size_t)md * W);
            }
            a7149_parts[a7149_np] = j;
            a7149_mult[a7149_np] = m;
            a7149_np++;
            a7149_rec(r2, j - 1, e2, v2, iz2, cnt + m);
            a7149_np--;
        }
    }
}

static int a7149_compute(int n)
{
    int i, k, m, res = 0, N1 = n + 1;
    a7149_N = n;
    a7149_gt = (int *)malloc(sizeof(int) * (size_t)N1 * (size_t)N1);
    a7149_L = (long long *)malloc(sizeof(long long) * (size_t)N1 * (size_t)N1);
    a7149_v2t = (int *)malloc(sizeof(int) * (size_t)N1);
    a7149_parts = (int *)malloc(sizeof(int) * (size_t)N1);
    a7149_mult = (int *)malloc(sizeof(int) * (size_t)N1);
    for (i = 0; i <= n; i++)
        for (k = 0; k <= n; k++)
            a7149_gt[i * N1 + k] = a7149_gcd(i, k);
    a7149_v2t[0] = 0;
    a7149_V2N = 0;
    for (i = 1; i <= n; i++) {
        int x = i, v = 0;
        while ((x & 1) == 0) { x >>= 1; v++; }
        a7149_v2t[i] = v;
        a7149_V2N += v;
    }
    for (i = 0; i <= n; i++) a7149_L[(long long)i * N1] = (i == 0) ? 0 : A7149_INF;
    for (k = 1; k <= n; k++) {
        for (i = 0; i <= n; i++) {
            long long best = a7149_L[(long long)i * N1 + k - 1];
            for (m = 1; m * k <= i; m++) {
                long long prev = a7149_L[(long long)(i - m * k) * N1 + k - 1];
                long long g;
                if (prev >= A7149_INF) continue;
                g = (long long)m * (k / 2) + (long long)m * (m - 1) / 2 * k
                    - (long long)m * a7149_v2t[k] - (m - a7149_popc(m));
                if (g + prev < best) best = g + prev;
            }
            a7149_L[(long long)i * N1 + k] = best;
        }
    }
    a7149_K = a7149_V2N + n / 2 + 8;
    for (;;) {
        uint64_t oddn[A7149_MAXW];
        int W = (int)(a7149_K / 64) + 1, found = 0;
        if (W > A7149_MAXW) { W = A7149_MAXW; a7149_K = 64LL * W - 1; }
        a7149_W = W;
        a7149_iod = (uint64_t *)malloc(sizeof(uint64_t) * (size_t)N1 * (size_t)W);
        for (i = 0; i < W; i++) a7149_iod[i] = (i == 0);
        for (i = 0; i < W; i++) oddn[i] = (i == 0);
        for (i = 1; i <= n; i++) {
            uint64_t x = (uint64_t)i, c = 0;
            while ((x & 1) == 0) x >>= 1;
            a7149_inv(a7149_iod + (size_t)i * W, x);
            for (k = 0; k < W; k++) {
                unsigned __int128 p = (unsigned __int128)oddn[k] * x + c;
                oddn[k] = (uint64_t)p;
                c = (uint64_t)(p >> 64);
            }
        }
        for (i = 0; i < W; i++) a7149_S[i] = 0;
        a7149_np = 0;
        a7149_rec(n, n, 0, 0, oddn, 0);
        free(a7149_iod);
        for (i = 0; i < W && !found; i++) {
            if (a7149_S[i]) {
                uint64_t s = a7149_S[i];
                long long b = 64LL * i;
                while ((s & 1) == 0) { s >>= 1; b++; }
                if (b < a7149_K) { res = (int)(b - a7149_V2N); found = 1; }
                break;
            }
        }
        if (found || W == A7149_MAXW) break;
        a7149_K += 32;
    }
    free(a7149_gt);
    free(a7149_L);
    free(a7149_v2t);
    free(a7149_parts);
    free(a7149_mult);
    return res;
}

seq_t a(idx_t n)
{
    static int *cache = NULL;
    static idx_t clen = 0;
    if (n >= clen) {
        idx_t nl = n + 1, i;
        cache = (int *)realloc(cache, sizeof(int) * (size_t)nl);
        for (i = clen; i < nl; i++) cache[i] = -1;
        clen = nl;
    }
    if (cache[n] < 0) cache[n] = a7149_compute((int)n);
    return (seq_t)cache[n];
}
