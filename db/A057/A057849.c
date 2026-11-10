/*
 * A057849  Primes p whose order of primeness A078442(p) is at least 7.
 * https://oeis.org/A057849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(prime(prime(prime(prime(prime(prime(n))))))), i.e.
 *            the 7-fold iterate of prime(); each prime(k) is found with a
 *            fast prime-counting function (min_25 style Lucy method,
 *            O(x^(3/4)/log x)) followed by a segmented sieve near the
 *            estimate
 * Range:     u8 none, u16 n <= 3, u32 n <= 137, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 44 terms through n = 9637 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#define NP_SMALL_LIM 1400000u
#define NP_SMALL_CAP 110000u

static uint32_t *np_small = NULL;
static size_t np_small_cnt = 0;

static void np_build_small(void)
{
    if (np_small) return;
    unsigned char *c = (unsigned char *)calloc(NP_SMALL_LIM + 1, 1);
    np_small = (uint32_t *)malloc(sizeof(uint32_t) * NP_SMALL_CAP);
    size_t cnt = 0;
    for (uint32_t i = 2; i <= NP_SMALL_LIM; i++) {
        if (!c[i]) {
            if (cnt < NP_SMALL_CAP) np_small[cnt++] = i;
            if ((uint64_t)i * i <= NP_SMALL_LIM)
                for (uint32_t j = i * i; j <= NP_SMALL_LIM; j += i) c[j] = 1;
        }
    }
    np_small_cnt = cnt;
    free(c);
}

static uint64_t np_isqrt(uint64_t x)
{
    uint64_t r = (uint64_t)sqrt((double)x);
    while (r > 0 && r * r > x) r--;
    while ((r + 1) * (r + 1) <= x) r++;
    return r;
}

static int64_t np_div(int64_t n, int64_t d)
{
    if (n < ((int64_t)1 << 50))
        return (int64_t)((double)n / (double)d);
    return n / d;
}

static int64_t np_half(int64_t n) { return (n - 1) >> 1; }

/* number of primes <= N */
static uint64_t np_prime_pi(uint64_t NN)
{
    if (NN < 2) return 0;
    if (NN == 2) return 1;
    int64_t N = (int64_t)NN;
    int64_t v = (int64_t)np_isqrt(NN);
    int64_t s = (v + 1) / 2;
    int32_t *smalls = (int32_t *)malloc(sizeof(int32_t) * (size_t)(s + 1));
    int32_t *roughs = (int32_t *)malloc(sizeof(int32_t) * (size_t)(s + 1));
    int64_t *larges = (int64_t *)malloc(sizeof(int64_t) * (size_t)(s + 1));
    unsigned char *skip = (unsigned char *)calloc((size_t)v + 2, 1);
    for (int64_t i = 0; i < s; i++) {
        smalls[i] = (int32_t)i;
        roughs[i] = (int32_t)(2 * i + 1);
        larges[i] = (N / (2 * i + 1) - 1) / 2;
    }
    int64_t pc = 0;
    for (int64_t p = 3; p <= v; p += 2) {
        if (skip[p]) continue;
        int64_t q = p * p;
        if (q * q > N) break;
        skip[p] = 1;
        for (int64_t i = q; i <= v; i += 2 * p) skip[i] = 1;
        int64_t ns = 0;
        for (int64_t k = 0; k < s; k++) {
            int64_t i = roughs[k];
            if (skip[i]) continue;
            int64_t d = i * p;
            int64_t sub = (d <= v) ? larges[smalls[d >> 1] - pc]
                                   : (int64_t)smalls[np_half(np_div(N, d))];
            larges[ns] = larges[k] - sub + pc;
            roughs[ns++] = (int32_t)i;
        }
        s = ns;
        int64_t i = np_half(v);
        for (int64_t j = ((v / p) - 1) | 1; j >= p; j -= 2) {
            int32_t c = (int32_t)(smalls[j >> 1] - pc);
            for (int64_t e = (j * p) >> 1; i >= e; --i) smalls[i] -= c;
        }
        ++pc;
    }
    larges[0] += (s + 2 * (pc - 1)) * (s - 1) / 2;
    for (int64_t k = 1; k < s; k++) larges[0] -= larges[k];
    for (int64_t l = 1; l < s; l++) {
        int64_t q = roughs[l];
        int64_t M = N / q;
        int64_t e = smalls[np_half(M / q)] - pc;
        if (e < l + 1) break;
        int64_t t = 0;
        for (int64_t k = l + 1; k <= e; k++)
            t += smalls[np_half(np_div(M, roughs[k]))];
        larges[0] += t - (e - l) * (pc + l - 1);
    }
    uint64_t res = (uint64_t)(larges[0] + 1);
    free(smalls);
    free(roughs);
    free(larges);
    free(skip);
    return res;
}

static double np_li(double x)
{
    double L = log(x);
    double s = 0.5772156649015329 + log(L);
    double term = 1.0;
    for (int n = 1; n < 300; n++) {
        term *= L / n;
        double add = term / n;
        s += add;
        if (add < 1e-17 * s && n > L) break;
    }
    return s;
}

static uint32_t *np_base = NULL;
static size_t np_base_cnt = 0;
static uint64_t np_base_lim = 0;

static void np_ensure_base(uint64_t hi)
{
    uint64_t need = np_isqrt(hi) + 2;
    if (np_base && np_base_lim >= need) return;
    free(np_base);
    uint64_t lim = need + need / 4 + 100;
    unsigned char *c = (unsigned char *)calloc(lim + 1, 1);
    np_base = (uint32_t *)malloc(sizeof(uint32_t) * (lim / 2 + 10));
    size_t cnt = 0;
    for (uint64_t i = 2; i <= lim; i++) {
        if (!c[i]) {
            np_base[cnt++] = (uint32_t)i;
            for (uint64_t j = i * i; j <= lim; j += i) c[j] = 1;
        }
    }
    np_base_cnt = cnt;
    np_base_lim = lim;
    free(c);
}

#define NP_W (1u << 21)

/* flags[i]==0 means lo+i is prime, for lo >= 2 */
static void np_segment(uint64_t lo, uint64_t hi, unsigned char *flags)
{
    uint64_t len = hi - lo + 1;
    for (uint64_t i = 0; i < len; i++) flags[i] = 0;
    for (size_t k = 0; k < np_base_cnt; k++) {
        uint64_t p = np_base[k];
        if (p * p > hi) break;
        uint64_t s = ((lo + p - 1) / p) * p;
        if (s < p * p) s = p * p;
        for (uint64_t j = s; j <= hi; j += p) flags[j - lo] = 1;
    }
}

static uint64_t np_nth_prime(uint64_t k)
{
    if (k <= 100000) {
        np_build_small();
        return np_small[k - 1];
    }
    double kd = (double)k;
    double lk = log(kd), llk = log(lk);
    double x = kd * (lk + llk - 1.0 + (llk - 2.0) / lk);
    for (int it = 0; it < 40; it++) {
        double f = np_li(x) - kd;
        double nx = x - f * log(x);
        if (fabs(nx - x) < 0.5) { x = nx; break; }
        x = nx;
    }
    uint64_t X = (uint64_t)x;
    if (X < 2) X = 2;
    uint64_t c = np_prime_pi(X);
    unsigned char *flags = (unsigned char *)malloc(NP_W);
    uint64_t result = 0;
    if (c < k) {
        uint64_t cnt = c;
        uint64_t lo = X + 1;
        for (;;) {
            uint64_t hi = lo + NP_W - 1;
            np_ensure_base(hi);
            np_segment(lo, hi, flags);
            for (uint64_t i = 0; i < NP_W; i++) {
                if (!flags[i]) {
                    cnt++;
                    if (cnt == k) { result = lo + i; goto done; }
                }
            }
            lo = hi + 1;
        }
    } else {
        uint64_t skipn = c - k;
        uint64_t hi = X;
        for (;;) {
            uint64_t lo = (hi + 1 > (uint64_t)NP_W + 2) ? hi + 1 - NP_W : 2;
            np_ensure_base(hi);
            np_segment(lo, hi, flags);
            for (uint64_t i = hi - lo + 1; i-- > 0;) {
                if (!flags[i]) {
                    if (skipn == 0) { result = lo + i; goto done; }
                    skipn--;
                }
            }
            hi = lo - 1;
        }
    }
done:
    free(flags);
    return result;
}

seq_t a(idx_t n)
{
    uint64_t v = (uint64_t)n;
    for (int i = 0; i < 7; i++) v = np_nth_prime(v);
    return (seq_t)v;
}
