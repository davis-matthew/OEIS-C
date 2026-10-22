/*
 * A001431  Number of graphs with n nodes and n-3 edges.
 * https://oeis.org/A001431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = [x^(n-3)] Z(S_n^{(2)}; 1+x), i.e. Polya counting over the
 *            cycle types lambda of S_n acting on vertex pairs: a(n) =
 *            Sum_{lambda |- n} (1/z_lambda) * [x^(n-3)] Prod_{pair orbits O}
 *            (1 + x^|O|), where an i-cycle and a j-cycle give gcd(i,j) orbits
 *            of size lcm(i,j), and a k-cycle by itself give...
 * Range:     u8 n <= 10, u16 n <= 15, u32 n <= 22, u64 n <= 35, u128 n <= 40,
 *            big any n
 * Helpers:   q1431_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/q1431_gcd.h"

typedef unsigned __int128 q1431_u128;

static uint64_t q_p;
static int q_n, q_m;
static uint64_t *q_inv, *q_invfact, *q_binr, *q_poly, *q_tmp, *q_coef;
static int *q_dv, *q_dc;
static int q_nd;
static uint64_t q_sum;

static uint64_t q1431_pmul(uint64_t a, uint64_t b, uint64_t m)
{
    return (uint64_t)((q1431_u128)a * b % m);
}

static uint64_t q1431_ppow(uint64_t b, uint64_t e, uint64_t m)
{
    uint64_t r = 1 % m;
    b %= m;
    while (e) {
        if (e & 1) r = q1431_pmul(r, b, m);
        b = q1431_pmul(b, b, m);
        e >>= 1;
    }
    return r;
}

static int q1431_isprime(uint64_t n)
{
    static const uint64_t bs[12] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    uint64_t d;
    int s, i, j;
    if (n < 2) return 0;
    for (i = 0; i < 12; i++) {
        if (n == bs[i]) return 1;
        if (n % bs[i] == 0) return 0;
    }
    d = n - 1;
    s = 0;
    while ((d & 1) == 0) { d >>= 1; s++; }
    for (i = 0; i < 12; i++) {
        uint64_t x = q1431_ppow(bs[i], d, n);
        if (x == 1 || x == n - 1) continue;
        for (j = 1; j < s; j++) {
            x = q1431_pmul(x, x, n);
            if (x == n - 1) break;
        }
        if (j == s) return 0;
    }
    return 1;
}

/* f *= (1 + x^s)^e, truncated at degree q_m, modulo q_p */
static void q1431_mulfac(uint64_t *f, int s, uint64_t e)
{
    int M = q_m, i, d;
    int terms;
    uint64_t passcost, bcost, p = q_p;
    if (e == 0 || s > M) return;
    terms = M / s;
    passcost = e * (uint64_t)(M - s + 1);
    bcost = 4ull * (uint64_t)(terms + 1) * (uint64_t)(M + 1);
    if (passcost <= bcost) {
        uint64_t q;
        for (q = 0; q < e; q++) {
            for (i = M; i >= s; i--) {
                uint64_t v = f[i] + f[i - s];
                if (v >= p) v -= p;
                f[i] = v;
            }
        }
    } else {
        int K = terms;
        if ((uint64_t)K > e) K = (int)e;
        q_coef[0] = 1;
        for (i = 1; i <= K; i++)
            q_coef[i] = q1431_pmul(q1431_pmul(q_coef[i - 1], (e - (uint64_t)i + 1) % p, p),
                                   q_inv[i], p);
        for (d = M; d >= 0; d--) {
            q1431_u128 acc = 0;
            int cnt = 0;
            for (i = 0; i <= K && i * s <= d; i++) {
                acc += (q1431_u128)q_coef[i] * f[d - i * s];
                if ((++cnt & 7) == 0) acc %= p;
            }
            f[d] = (uint64_t)(acc % p);
        }
    }
}

static void q1431_leaf(int depth, int t, uint64_t w)
{
    int r = q_n - t, i, M1 = q_m + 1;
    uint64_t *f = q_poly + (size_t)depth * M1;
    uint64_t c;
    if (r == 0) {
        c = f[q_m];
    } else {
        const uint64_t *B = q_binr + (size_t)r * M1;
        q1431_u128 acc = 0;
        memcpy(q_tmp, f, (size_t)M1 * sizeof(uint64_t));
        for (i = 0; i < q_nd; i++)
            q1431_mulfac(q_tmp, q_dv[i], (uint64_t)r * (uint64_t)q_dc[i]);
        for (i = 0; i <= q_m; i++) {
            acc += (q1431_u128)q_tmp[q_m - i] * B[i];
            if ((i & 7) == 7) acc %= q_p;
        }
        c = (uint64_t)(acc % q_p);
    }
    q_sum = (q_sum + q1431_pmul(q1431_pmul(c, w, q_p), q_invfact[r], q_p)) % q_p;
}

static void q1431_dfs(int depth, int t, int maxk, uint64_t w)
{
    int k, i, M1 = q_m + 1;
    int top = q_n - t;
    q1431_leaf(depth, t, w);
    if (top > maxk) top = maxk;
    for (k = top; k >= 2; k--) {
        uint64_t *f = q_poly + (size_t)(depth + 1) * M1;
        int pushed;
        uint64_t c, w2;
        memcpy(f, q_poly + (size_t)depth * M1, (size_t)M1 * sizeof(uint64_t));
        q1431_mulfac(f, k, (uint64_t)((k - 1) / 2));
        if (k % 2 == 0) q1431_mulfac(f, k / 2, 1);
        for (i = 0; i < q_nd; i++) {
            int j = q_dv[i];
            int g = q1431_gcd(j, k);
            long long l = (long long)(j / g) * k;
            if (l <= q_m) q1431_mulfac(f, (int)l, (uint64_t)g * (uint64_t)q_dc[i]);
        }
        if (q_nd > 0 && q_dv[q_nd - 1] == k) {
            q_dc[q_nd - 1]++;
            c = (uint64_t)q_dc[q_nd - 1];
            pushed = 0;
        } else {
            q_dv[q_nd] = k;
            q_dc[q_nd] = 1;
            q_nd++;
            c = 1;
            pushed = 1;
        }
        w2 = q1431_pmul(q1431_pmul(w, q_inv[k], q_p), q_inv[c], q_p);
        q1431_dfs(depth + 1, t + k, k, w2);
        if (pushed) q_nd--;
        else q_dc[q_nd - 1]--;
    }
}

static uint64_t q1431_run(uint64_t p)
{
    int L = (q_n > q_m ? q_n : q_m) + 1;
    int i, r, M1 = q_m + 1;
    q_p = p;
    q_inv[1] = 1;
    for (i = 2; i <= L; i++)
        q_inv[i] = q1431_pmul(p - p / (uint64_t)i, q_inv[p % (uint64_t)i], p);
    q_invfact[0] = 1;
    for (i = 1; i <= q_n; i++) q_invfact[i] = q1431_pmul(q_invfact[i - 1], q_inv[i], p);
    for (r = 0; r <= q_n; r++) {
        uint64_t N = (uint64_t)r * (uint64_t)(r - 1 < 0 ? 0 : r - 1) / 2;
        uint64_t *B = q_binr + (size_t)r * M1;
        B[0] = 1;
        for (i = 1; i <= q_m; i++) {
            if ((uint64_t)i > N) B[i] = 0;
            else B[i] = q1431_pmul(q1431_pmul(B[i - 1], (N - (uint64_t)i + 1) % p, p), q_inv[i], p);
        }
    }
    memset(q_poly, 0, (size_t)M1 * sizeof(uint64_t));
    q_poly[0] = 1;
    q_sum = 0;
    q_nd = 0;
    q1431_dfs(0, 0, q_n, 1);
    return q_sum;
}

seq_t a(idx_t n)
{
    uint64_t pr[64], dg[64];
    int K = 0, maxK, bl, i, L, M1;
    uint64_t cand = (1ULL << 62);
    long long B;
    seq_t res;
    if (n < 3) return (seq_t)0;
    q_n = (int)n;
    q_m = (int)(n - 3);
    M1 = q_m + 1;
    L = (q_n > q_m ? q_n : q_m) + 2;
    q_inv = (uint64_t *)malloc((size_t)L * sizeof(uint64_t));
    q_invfact = (uint64_t *)malloc((size_t)(q_n + 1) * sizeof(uint64_t));
    q_binr = (uint64_t *)malloc((size_t)(q_n + 1) * M1 * sizeof(uint64_t));
    q_poly = (uint64_t *)malloc((size_t)(q_n / 2 + 2) * M1 * sizeof(uint64_t));
    q_tmp = (uint64_t *)malloc((size_t)M1 * sizeof(uint64_t));
    q_coef = (uint64_t *)malloc((size_t)(M1 + 1) * sizeof(uint64_t));
    q_dv = (int *)malloc((size_t)(q_n + 1) * sizeof(int));
    q_dc = (int *)malloc((size_t)(q_n + 1) * sizeof(int));

    bl = 0;
    { uint64_t x = (uint64_t)n; while (x) { bl++; x >>= 1; } }
    B = (long long)q_m * 2 * bl;           /* a(n) <= C(C(n,2),m) < 2^B */
    maxK = (int)(B / 61) + 1;
    if (maxK > 64) maxK = 64;
    if (maxK < 1) maxK = 1;

    for (;;) {
        uint64_t r, v, prod, diff, d;
        do { cand--; } while (!q1431_isprime(cand));
        r = q1431_run(cand);
        if (K == 0) {
            pr[0] = cand;
            dg[0] = r;
            K = 1;
            if (K >= maxK) break;
            continue;
        }
        v = 0;
        for (i = K - 1; i >= 0; i--)
            v = (q1431_pmul(v, pr[i] % cand, cand) + dg[i] % cand) % cand;
        prod = 1;
        for (i = 0; i < K; i++) prod = q1431_pmul(prod, pr[i] % cand, cand);
        diff = (r + cand - v) % cand;
        d = q1431_pmul(diff, q1431_ppow(prod, cand - 2, cand), cand);
        if (d == 0) break;
        pr[K] = cand;
        dg[K] = d;
        K++;
        if (K >= maxK) break;
    }

    res = (seq_t)dg[K - 1];
    for (i = K - 2; i >= 0; i--)
        res = res * (seq_t)pr[i] + (seq_t)dg[i];

    free(q_inv); free(q_invfact); free(q_binr); free(q_poly);
    free(q_tmp); free(q_coef); free(q_dv); free(q_dc);
    return res;
}
