/*
 * A005417  Maximal period of an n-stage shift register.
 * https://oeis.org/A005417
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = max m such that A067240(m) <= 2n + 1, i.e. the largest
 *            product of prime powers p^k (distinct p) whose total cost is <=
 *            2n+1, where cost(p^k) = phi(p^k) and cost(2) = 0. Computed by a
 *            knapsack search over primes (log scale for the choices), then
 *            the chosen prime powers are multiplied exactl...
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 50, u64 n <= 164, u128 n <= 528,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

static long double *kn_dp = NULL;
static unsigned char *kn_ch = NULL;
static uint64_t *kn_pr = NULL;
static long kn_np = 0;
static uint64_t kn_cap = 0;

static void kn_build(uint64_t B) {
    uint64_t i, c;
    long idx;
    char *sv;
    if (kn_dp) free(kn_dp);
    if (kn_ch) free(kn_ch);
    if (kn_pr) free(kn_pr);
    kn_cap = B;
    sv = (char *)calloc(B + 2, 1);
    kn_pr = (uint64_t *)malloc(sizeof(uint64_t) * (B + 2));
    kn_np = 0;
    for (i = 2; i <= B + 1; i++) {
        if (!sv[i]) {
            uint64_t j;
            kn_pr[kn_np++] = i;
            for (j = i * i; j <= B + 1; j += i) sv[j] = 1;
        }
    }
    free(sv);
    kn_dp = (long double *)calloc(B + 1, sizeof(long double));
    kn_ch = (unsigned char *)calloc((size_t)kn_np * (B + 1), 1);
    for (idx = 0; idx < kn_np; idx++) {
        uint64_t p = kn_pr[idx];
        long double lp = logl((long double)p);
        unsigned char *row = kn_ch + (size_t)idx * (B + 1);
        for (c = B + 1; c-- > 0;) {
            long double best = kn_dp[c];
            int bk = 0;
            uint64_t pk = p;
            uint64_t cost = (p == 2) ? 0 : p - 1;
            long double lg = lp;
            int k = 1;
            while (cost <= c) {
                long double v = kn_dp[c - cost] + lg;
                if (v > best) { best = v; bk = k; }
                pk *= p;
                cost = pk - pk / p;
                lg += lp;
                k++;
            }
            kn_dp[c] = best;
            row[c] = (unsigned char)bk;
        }
    }
}

seq_t a(idx_t n) {
    uint64_t B = 2 * (uint64_t)n + 1;
    uint64_t c;
    long idx;
    seq_t r = 1;
    if (kn_dp == NULL || B > kn_cap) {
        uint64_t want = B < 20003 ? 20003 : B;
        kn_build(want);
    }
    c = B;
    for (idx = kn_np - 1; idx >= 0; idx--) {
        int k = kn_ch[(size_t)idx * (kn_cap + 1) + c];
        if (k) {
            uint64_t p = kn_pr[idx];
            uint64_t pk = 1;
            int j;
            for (j = 0; j < k; j++) {
                r *= (seq_t)p;
                pk *= p;
            }
            if (p == 2 && k == 1) {
                /* free */
            } else {
                c -= pk - pk / p;
            }
        }
    }
    return r;
}
