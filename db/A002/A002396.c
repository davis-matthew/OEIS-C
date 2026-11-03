/*
 * A002396  Inverse of reduced totient function.
 * https://oeis.org/A002396
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   search, not a closed form. a(n) = least m with lambda(m) = k,
 *            where k runs through the values taken by the Carmichael function
 *            lambda (A002322), in increasing order. For each k: m is a
 *            product of prime powers p^e with lambda(p^e) | k, and lcm of
 *            those lambda(p^e) must equal k. A knapsack over the...
 * Range:     u8 n <= 23, u16 n <= 607, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 75 terms through n = 883 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"

static uint64_t a2396_mulsat(uint64_t x, uint64_t y) {
    if (x == UINT64_MAX || y == UINT64_MAX) return UINT64_MAX;
    if (y != 0 && x > UINT64_MAX / y) return UINT64_MAX;
    return x * y;
}

/* returns least m with lambda(m) = k, or 0 if none */
static uint64_t a2396_minm(uint64_t k) {
    if (k == 1) return 1;
    if (k & 1) return 0;
    uint64_t q[16]; int f[16]; int w = 0;
    uint64_t t = k;
    for (uint64_t p = 2; p * p <= t; p++) {
        if (t % p == 0) {
            int e = 0;
            while (t % p == 0) { t /= p; e++; }
            q[w] = p; f[w] = e; w++;
        }
    }
    if (t > 1) { q[w] = t; f[w] = 1; w++; }
    int full = (1 << w) - 1;
    uint64_t dp[64], nd[64];
    for (int i = 0; i <= full; i++) dp[i] = UINT64_MAX;
    dp[0] = 1;

    /* divisors of k */
    uint64_t *divs = (uint64_t *)malloc(sizeof(uint64_t) * 4096);
    int nd_ = 1; divs[0] = 1;
    for (int i = 0; i < w; i++) {
        int cur = nd_;
        uint64_t pw = 1;
        for (int e = 1; e <= f[i]; e++) {
            pw *= q[i];
            for (int j = 0; j < cur; j++) divs[nd_++] = divs[j] * pw;
        }
    }

    /* prime 2 group */
    {
        for (int i = 0; i <= full; i++) nd[i] = dp[i];
        uint64_t val = 4;
        int e = 2;
        while (1) {
            uint64_t lam = (e == 2) ? 2 : ((uint64_t)1 << (e - 2));
            if (k % lam != 0) break;
            int mask = 0;
            int v2 = (e == 2) ? 1 : e - 2;
            for (int i = 0; i < w; i++)
                if (q[i] == 2 && v2 == f[i]) mask |= 1 << i;
            if (mask)
                for (int m = 0; m <= full; m++)
                    if (dp[m] != UINT64_MAX) {
                        uint64_t c = a2396_mulsat(dp[m], val);
                        if (c < nd[m | mask]) nd[m | mask] = c;
                    }
            if (val > UINT64_MAX / 2) break;
            val *= 2; e++;
        }
        for (int i = 0; i <= full; i++) dp[i] = nd[i];
    }

    /* odd primes p with p-1 | k */
    for (int di = 0; di < nd_; di++) {
        uint64_t d = divs[di];
        if (d < 2) continue;
        uint64_t p = d + 1;
        if (!is_prime_u64(p)) continue;
        for (int i = 0; i <= full; i++) nd[i] = dp[i];
        uint64_t val = p;
        int e = 1;
        while (1) {
            /* lambda(p^e) = p^(e-1)(p-1); need p^(e-1) | k */
            int mask = 0;
            for (int i = 0; i < w; i++) {
                int v = 0; uint64_t x = d;
                while (x % q[i] == 0) { x /= q[i]; v++; }
                if (q[i] == p) v += e - 1;
                if (v == f[i]) mask |= 1 << i;
            }
            if (mask)
                for (int m = 0; m <= full; m++)
                    if (dp[m] != UINT64_MAX) {
                        uint64_t c = a2396_mulsat(dp[m], val);
                        if (c < nd[m | mask]) nd[m | mask] = c;
                    }
            /* next exponent requires p^e | k */
            uint64_t pe = 1;
            for (int j = 0; j < e; j++) pe *= p;
            if (k % pe != 0) break;
            if (val > UINT64_MAX / p) break;
            val *= p; e++;
        }
        for (int i = 0; i <= full; i++) dp[i] = nd[i];
    }
    free(divs);
    return dp[full] == UINT64_MAX ? 0 : dp[full];
}

static uint64_t *a2396_res = NULL;
static idx_t a2396_cnt = 0, a2396_cap = 0;
static uint64_t a2396_nextk = 1;

seq_t a(idx_t n) {
    while (a2396_cnt < n) {
        uint64_t m = a2396_minm(a2396_nextk);
        a2396_nextk++;
        if (m == 0) continue;
        if (a2396_cnt == a2396_cap) {
            idx_t nc = a2396_cap ? a2396_cap * 2 : 1024;
            a2396_res = (uint64_t *)realloc(a2396_res, sizeof(uint64_t) * nc);
            a2396_cap = nc;
        }
        a2396_res[a2396_cnt++] = m;
    }
    return (seq_t)a2396_res[n - 1];
}
