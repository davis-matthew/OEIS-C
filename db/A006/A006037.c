/*
 * A006037  Weird numbers: abundant (A005101) but not pseudoperfect (A005835).
 * https://oeis.org/A006037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   search, not a closed form. Scan even k (there are no odd weird
 *            numbers below 10^17). Skip multiples of 6, 20 and 28, which are
 *            pseudoperfect. If sigma(k) > 2k, let e = sigma(k) - 2k. k is
 *            pseudoperfect iff some proper divisors <= e sum to exactly e
 *            (the complement then sums to k). This subset sum...
 * Range:     u8 n <= 1, u16 n <= 134, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   w_cmp_desc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/w_cmp_desc.h"

static uint32_t *w_spf = NULL;
static uint64_t w_spf_lim = 0;
static uint64_t *w_terms = NULL;
static uint64_t w_cnt = 0, w_cap = 0;
static uint64_t w_next = 2;

static uint64_t w_div[8192];
static uint64_t w_suf[8193];
static int w_k;
static long w_budget;

static void w_build_spf(uint64_t L) {
    free(w_spf);
    w_spf = (uint32_t *)calloc((size_t)L + 1, sizeof(uint32_t));
    for (uint64_t i = 2; i <= L; i++) {
        if (w_spf[i] == 0) {
            for (uint64_t j = i; j <= L; j += i)
                if (w_spf[j] == 0) w_spf[j] = (uint32_t)i;
        }
    }
    w_spf_lim = L;
}

/* 1 = found, 0 = impossible, -1 = budget exhausted */
static int w_dfs(uint64_t t, int i) {
    if (t == 0) return 1;
    while (i < w_k && w_div[i] > t) i++;
    if (i >= w_k) return 0;
    if (w_suf[i] < t) return 0;
    if (w_suf[i] == t) return 1;
    if (--w_budget < 0) return -1;
    int r = w_dfs(t - w_div[i], i + 1);
    if (r != 0) return r;
    return w_dfs(t, i + 1);
}

static bool w_dp(uint64_t e) {
    uint64_t W = e / 64 + 1;
    uint64_t *b = (uint64_t *)calloc((size_t)W, sizeof(uint64_t));
    b[0] = 1;
    for (int t = 0; t < w_k; t++) {
        uint64_t d = w_div[t];
        uint64_t ws = d / 64, bs = d % 64;
        for (uint64_t w = W; w-- > ws;) {
            uint64_t src = w - ws;
            uint64_t v = b[src] << bs;
            if (bs && src >= 1) v |= b[src - 1] >> (64 - bs);
            b[w] |= v;
        }
        if ((b[e / 64] >> (e % 64)) & 1) break;
    }
    bool res = ((b[e / 64] >> (e % 64)) & 1) != 0;
    free(b);
    return res;
}

static bool w_is_weird(uint64_t c) {
    uint64_t p[32];
    int ex[32];
    int np = 0;
    uint64_t m = c, sigma = 1;
    while (m > 1) {
        uint64_t q = w_spf[m];
        int k = 0;
        uint64_t pk = 1, s = 1;
        while (m % q == 0) { m /= q; k++; pk *= q; s += pk; }
        p[np] = q; ex[np] = k; np++;
        sigma *= s;
    }
    if (sigma <= 2 * c) return false;
    uint64_t e = sigma - 2 * c;

    /* generate divisors */
    int cnt = 1;
    w_div[0] = 1;
    for (int i = 0; i < np; i++) {
        int cur = cnt;
        uint64_t pk = 1;
        for (int j = 1; j <= ex[i]; j++) {
            pk *= p[i];
            for (int t = 0; t < cur; t++) w_div[cnt++] = w_div[t] * pk;
        }
    }
    int k = 0;
    for (int t = 0; t < cnt; t++)
        if (w_div[t] < c && w_div[t] <= e) w_div[k++] = w_div[t];
    w_k = k;
    qsort(w_div, (size_t)k, sizeof(uint64_t), w_cmp_desc);
    w_suf[k] = 0;
    for (int t = k - 1; t >= 0; t--) w_suf[t] = w_suf[t + 1] + w_div[t];
    if (w_suf[0] < e) return true;

    w_budget = 200000;
    int r = w_dfs(e, 0);
    if (r == 1) return false;
    if (r == 0) return true;
    return !w_dp(e);
}

seq_t a(idx_t n) {
    while (w_cnt < n) {
        if (w_next > w_spf_lim) {
            uint64_t L = w_spf_lim ? w_spf_lim * 2 : ((uint64_t)1 << 16);
            w_build_spf(L);
        }
        uint64_t c = w_next;
        w_next += 2;
        if (c % 6 == 0 || c % 20 == 0 || c % 28 == 0) continue;
        if (w_is_weird(c)) {
            if (w_cnt == w_cap) {
                w_cap = w_cap ? w_cap * 2 : 1024;
                w_terms = (uint64_t *)realloc(w_terms, (size_t)w_cap * sizeof(uint64_t));
            }
            w_terms[w_cnt++] = c;
        }
    }
    return (seq_t)w_terms[n - 1];
}
