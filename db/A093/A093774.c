/*
 * A093774  Smallest integers at which the value of truncated Mertens function
 *          equals n!.
 * https://oeis.org/A093774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = Min(x : A002321(x) + A000720(x) = n!). Computed by
 *            search: F(x) = M(x) + pi(x) is evaluated exactly at a safe
 *            starting point x0 (Lucy_Hedgehog prime count plus Mertens via
 *            the M(v) = 1 - Sum_{d>=2} M(floor(v/d)) recursion). x0 is chosen
 *            so that pi(x0) + sqrt(x0) < n!. Since |M(x)| < sqrt(x...
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Validated: partial, pass, 13 terms through n = 13 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

static uint64_t A093774_isqrt(uint64_t x) {
    uint64_t r = (uint64_t)sqrt((double)x);
    while (r > 0 && r * r > x) r--;
    while ((r + 1) * (r + 1) <= x) r++;
    return r;
}

static double A093774_li(double x) {
    double L = log(x), s = 0, t = 1;
    for (int k = 0; k <= 12; k++) {
        s += t;
        t *= (double)(k + 1) / L;
    }
    return x / L * s;
}

static double A093774_inv_li(double T) {
    double lo = 1e6, hi = 1e15;
    for (int i = 0; i < 200; i++) {
        double mid = 0.5 * (lo + hi);
        if (A093774_li(mid) < T) lo = mid; else hi = mid;
    }
    return 0.5 * (lo + hi);
}

static int64_t A093774_pi(uint64_t x) {
    uint64_t r = A093774_isqrt(x);
    int64_t *lo = (int64_t *)calloc(r + 2, sizeof(int64_t));
    int64_t *hi = (int64_t *)calloc(r + 2, sizeof(int64_t));
    if (!lo || !hi) { free(lo); free(hi); return 0; }
    for (uint64_t i = 1; i <= r; i++) {
        lo[i] = (int64_t)i - 1;
        hi[i] = (int64_t)(x / i) - 1;
    }
    for (uint64_t p = 2; p <= r; p++) {
        if (lo[p] == lo[p - 1]) continue;
        int64_t pc = lo[p - 1];
        uint64_t p2 = p * p;
        if (p2 > x) break;
        uint64_t lim = x / p2;
        if (lim > r) lim = r;
        for (uint64_t i = 1; i <= lim; i++) {
            uint64_t d = i * p;
            int64_t v;
            if (d <= r) v = hi[d];
            else v = lo[x / d];
            hi[i] -= v - pc;
        }
        for (uint64_t i = r; i >= p2; i--) {
            lo[i] -= lo[i / p] - pc;
        }
    }
    int64_t res = hi[1];
    free(lo); free(hi);
    return res;
}

static int64_t A093774_mertens(uint64_t x) {
    uint64_t y = (uint64_t)pow((double)x, 0.62);
    if (y < 100) y = 100;
    if (y > x) y = x;
    int8_t *mu = (int8_t *)calloc(y + 1, 1);
    uint8_t *comp = (uint8_t *)calloc(y + 1, 1);
    int32_t *ms = (int32_t *)calloc(y + 1, sizeof(int32_t));
    uint32_t *pr = (uint32_t *)malloc(sizeof(uint32_t) * (size_t)(y / 4 + 100));
    size_t pc = 0;
    mu[1] = 1;
    for (uint64_t i = 2; i <= y; i++) {
        if (!comp[i]) { pr[pc++] = (uint32_t)i; mu[i] = -1; }
        for (size_t j = 0; j < pc; j++) {
            uint64_t prod = i * pr[j];
            if (prod > y) break;
            comp[prod] = 1;
            if (i % pr[j] == 0) { mu[prod] = 0; break; }
            mu[prod] = (int8_t)-mu[i];
        }
    }
    ms[0] = 0;
    for (uint64_t i = 1; i <= y; i++) ms[i] = ms[i - 1] + mu[i];
    int64_t res;
    if (x <= y) {
        res = ms[x];
    } else {
        uint64_t K = x / y;
        int64_t *big = (int64_t *)calloc(K + 2, sizeof(int64_t));
        for (uint64_t k = K; k >= 1; k--) {
            uint64_t v = x / k;
            if (v <= y) { big[k] = ms[v]; continue; }
            int64_t sum = 0;
            for (uint64_t d = 2; d <= v;) {
                uint64_t q = v / d;
                uint64_t d2 = v / q;
                int64_t Mq = (q <= y) ? (int64_t)ms[q] : big[k * d];
                sum += (int64_t)(d2 - d + 1) * Mq;
                d = d2 + 1;
            }
            big[k] = 1 - sum;
        }
        res = big[1];
        free(big);
    }
    free(mu); free(comp); free(ms); free(pr);
    return res;
}

seq_t a(idx_t n) {
    if (n < 1 || n > 15) return 0;
    seq_t Ns = 1;
    for (idx_t i = 2; i <= n; i++) Ns *= (seq_t)i;
    int64_t N = (int64_t)(long long)Ns;

    uint64_t xlo = 0;
    int64_t F = 0;
    if (N >= 3000000) {
        double xe = A093774_inv_li((double)N);
        double xl = A093774_inv_li((double)N - 1.3 * sqrt(xe));
        for (int tries = 0; tries < 40; tries++) {
            if (xl < 1e7) { xlo = 0; F = 0; break; }
            uint64_t cand = (uint64_t)xl;
            int64_t P = A093774_pi(cand);
            if (P + (int64_t)A093774_isqrt(cand) + 2 < N) {
                xlo = cand;
                F = A093774_mertens(cand) + P;
                break;
            }
            xl *= 0.97;
        }
    }

    uint64_t B = A093774_isqrt(4 * xlo > 100000000ULL ? 4 * xlo : 100000000ULL) + 10;
    uint8_t *sv = (uint8_t *)calloc(B + 1, 1);
    uint32_t *pl = (uint32_t *)malloc(sizeof(uint32_t) * (size_t)(B / 2 + 10));
    size_t np = 0;
    for (uint64_t i = 2; i <= B; i++) {
        if (!sv[i]) {
            pl[np++] = (uint32_t)i;
            for (uint64_t j = i * i; j <= B; j += i) sv[j] = 1;
        }
    }
    free(sv);

    const uint64_t S = 1 << 17;
    uint64_t *prod = (uint64_t *)malloc(S * sizeof(uint64_t));
    uint8_t *cnt = (uint8_t *)malloc(S);
    uint8_t *sq = (uint8_t *)malloc(S);
    uint64_t L = xlo + 1;
    uint64_t ans = 0;
    bool found = false;
    while (!found) {
        uint64_t H = L + S;  /* exclusive */
        if (H > B * B) break;
        for (uint64_t i = 0; i < S; i++) { prod[i] = 1; cnt[i] = 0; sq[i] = 0; }
        for (size_t j = 0; j < np; j++) {
            uint64_t p = pl[j];
            if (p * p >= H) break;
            uint64_t st = (L + p - 1) / p * p;
            for (uint64_t m = st; m < H; m += p) {
                prod[m - L] *= p;
                cnt[m - L]++;
            }
            uint64_t p2 = p * p;
            uint64_t s2 = (L + p2 - 1) / p2 * p2;
            for (uint64_t m = s2; m < H; m += p2) sq[m - L] = 1;
        }
        for (uint64_t i = 0; i < S; i++) {
            if (sq[i]) continue;
            uint64_t xv = L + i;
            unsigned k = cnt[i] + (prod[i] != xv ? 1u : 0u);
            if (k == 1) continue;
            F += (k & 1) ? -1 : 1;
            if (F == N) { ans = xv; found = true; break; }
        }
        L = H;
    }
    free(pl); free(prod); free(cnt); free(sq);
    return (seq_t)ans;
}
