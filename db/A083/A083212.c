/*
 * A083212  Where records occur in A083206.
 * https://oeis.org/A083212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search over k with a(1) = 1, a(n+1) = min{k > a(n) : A083206(k)
 *            > A083206(a(n))}. A083206(k) is the number of subsets of the
 *            divisors of k, other than k itself, that sum to sigma(k)/2 - k.
 *            It is computed by a subset-sum DP in double precision; near-ties
 *            are checked exactly with multi-limb integer...
 * Range:     u8 n <= 8, u16 n <= 24, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Validated: partial, pass, 30 terms through n = 30 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

static uint16_t *A083212_dc = NULL;   /* number of divisors sieve */
static uint64_t A083212_dlim = 0;

static void A083212_ensure_sieve(uint64_t k)
{
    if (k <= A083212_dlim) return;
    uint64_t lim = A083212_dlim ? A083212_dlim : 32768;
    while (lim < k) lim *= 2;
    free(A083212_dc);
    A083212_dc = (uint16_t *)calloc((size_t)lim + 1, sizeof(uint16_t));
    for (uint64_t i = 1; i <= lim; ++i)
        for (uint64_t j = i; j <= lim; j += i)
            A083212_dc[j]++;
    A083212_dlim = lim;
}

/* sorted ascending divisors of k; returns count */
static size_t A083212_divs(uint64_t k, uint64_t *dv)
{
    static uint64_t hi[8192];
    size_t lo = 0, hc = 0;
    for (uint64_t i = 1; i * i <= k; ++i) {
        if (k % i == 0) {
            dv[lo++] = i;
            if (i != k / i) hi[hc++] = k / i;
        }
    }
    while (hc > 0) dv[lo++] = hi[--hc];
    return lo;
}

static double *A083212_dbuf = NULL;
static size_t A083212_dcap = 0;

static double *A083212_getbuf(size_t n)
{
    if (n > A083212_dcap) {
        free(A083212_dbuf);
        A083212_dbuf = (double *)malloc(n * sizeof(double));
        A083212_dcap = n;
    }
    return A083212_dbuf;
}

/* number of subsets of dv[0..m) summing to T, in double */
static double A083212_dp_full(const uint64_t *dv, size_t m, size_t T)
{
    double *dp = A083212_getbuf(T + 1);
    for (size_t s = 0; s <= T; ++s) dp[s] = 0.0;
    dp[0] = 1.0;
    for (size_t i = 0; i < m; ++i) {
        size_t d = (size_t)dv[i];
        if (d > T) break;
        for (size_t s = T; s >= d; --s)
            dp[s] += dp[s - d];
    }
    return dp[T];
}

/* rigorous upper bound: 2^(m - j) * max_t #(subsets of dv[0..j) summing to t) */
static double A083212_bound(const uint64_t *dv, size_t m, size_t cap)
{
    size_t j = 0, sum = 0;
    while (j < m && sum + (size_t)dv[j] <= cap) { sum += (size_t)dv[j]; ++j; }
    double *dp = A083212_getbuf(sum + 1);
    for (size_t s = 0; s <= sum; ++s) dp[s] = 0.0;
    dp[0] = 1.0;
    size_t cur = 0;
    for (size_t i = 0; i < j; ++i) {
        size_t d = (size_t)dv[i];
        cur += d;
        for (size_t s = cur; s >= d; --s)
            dp[s] += dp[s - d];
    }
    double mx = 0.0;
    for (size_t s = 0; s <= sum; ++s) if (dp[s] > mx) mx = dp[s];
    return ldexp(mx, (int)(m - j)) * (1.0 + 1e-9);
}

/* exact count for k into out[0..L) (little-endian limbs) */
static void A083212_exact(uint64_t k, uint64_t *out, size_t L)
{
    static uint64_t dv[8192];
    size_t D = A083212_divs(k, dv);
    uint64_t sg = 0;
    for (size_t i = 0; i < D; ++i) sg += dv[i];
    for (size_t l = 0; l < L; ++l) out[l] = 0;
    if ((sg & 1) || sg < 2 * k) return;
    size_t T = (size_t)(sg / 2 - k);
    uint64_t *dp = (uint64_t *)calloc((T + 1) * L, sizeof(uint64_t));
    dp[0] = 1;
    for (size_t i = 0; i + 1 < D; ++i) {
        size_t d = (size_t)dv[i];
        if (d > T) break;
        for (size_t s = T; s >= d; --s) {
            uint64_t *x = dp + s * L;
            const uint64_t *y = dp + (s - d) * L;
            uint64_t c = 0;
            for (size_t l = 0; l < L; ++l) {
                uint64_t t = x[l] + y[l];
                uint64_t c1 = (t < x[l]);
                uint64_t t2 = t + c;
                uint64_t c2 = (t2 < t);
                x[l] = t2;
                c = c1 | c2;
            }
        }
    }
    memcpy(out, dp + T * L, L * sizeof(uint64_t));
    free(dp);
}

/* returns 1 if count(k1) > count(k2) exactly */
static int A083212_exact_greater(uint64_t k1, uint64_t k2)
{
    A083212_ensure_sieve(k1 > k2 ? k1 : k2);
    size_t m1 = A083212_dc[k1], m2 = A083212_dc[k2];
    size_t L = (m1 > m2 ? m1 : m2) / 64 + 2;
    uint64_t *x = (uint64_t *)calloc(L, sizeof(uint64_t));
    uint64_t *y = (uint64_t *)calloc(L, sizeof(uint64_t));
    A083212_exact(k1, x, L);
    A083212_exact(k2, y, L);
    int res = 0;
    for (size_t l = L; l-- > 0;) {
        if (x[l] != y[l]) { res = (x[l] > y[l]); break; }
    }
    free(x);
    free(y);
    return res;
}

seq_t a(idx_t n)
{
    static uint64_t *recs = NULL;
    static size_t rcnt = 0, rcap = 0;
    static uint64_t cur = 1;
    static double best = 0.0;
    static uint64_t bestk = 1;
    static uint64_t dv[8192];

    if (rcnt == 0) {
        rcap = 64;
        recs = (uint64_t *)malloc(rcap * sizeof(uint64_t));
        recs[rcnt++] = 1;
        cur = 1;
        best = 0.0;    /* A083206(1) = 0 */
        bestk = 1;
    }
    while (rcnt < (size_t)n) {
        ++cur;
        A083212_ensure_sieve(cur);
        size_t m = (size_t)A083212_dc[cur] - 1;
        if (best > 0.0 && ldexp(1.0, (int)m) < best * (1.0 - 1e-8))
            continue;
        size_t D = A083212_divs(cur, dv);
        uint64_t sg = 0;
        for (size_t i = 0; i < D; ++i) sg += dv[i];
        if ((sg & 1) || sg < 2 * cur) continue;
        size_t T = (size_t)(sg / 2 - cur);
        if (best > 0.0 && T > 20000) {
            double b = A083212_bound(dv, D - 1, T / 8);
            if (b < best * (1.0 - 1e-8)) continue;
        }
        double c = A083212_dp_full(dv, D - 1, T);
        int rec = 0;
        if (c > best * (1.0 + 1e-9)) rec = 1;
        else if (best > 0.0 && c >= best * (1.0 - 1e-9))
            rec = A083212_exact_greater(cur, bestk);
        if (rec) {
            best = c;
            bestk = cur;
            if (rcnt == rcap) {
                rcap *= 2;
                recs = (uint64_t *)realloc(recs, rcap * sizeof(uint64_t));
            }
            recs[rcnt++] = cur;
        }
    }
    return (seq_t)recs[n - 1];
}
