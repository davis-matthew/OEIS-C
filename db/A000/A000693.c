/*
 * A000693  Related to population of numbers of form x^2 + y^2.
 * https://oeis.org/A000693
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(2) = 2, a(n) = A000074(n) + a(n-2) for n > 2, where
 *            A000074(n) = number of odd m <= 2^n with m = x^2 + y^2, i.e. odd
 *            m whose primes 3 (mod 4) all have even exponent; computed by a
 *            Lucy/min_25 sieve (prime counts by residue class mod 4, then
 *            multiplicative summation)
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   my_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/my_isqrt.h"

static uint64_t g_x, g_sq;
static int64_t *lo_pc, *hi_pc, *lo_sc, *hi_sc;
static uint64_t *pr;
static long long npr;

/* number of primes p <= v with p == 1 (mod 4) */
static int64_t fp(uint64_t v) {
    int64_t pc, sc;
    if (v < 2) return 0;
    if (v <= g_sq) { pc = lo_pc[v]; sc = lo_sc[v]; }
    else { pc = hi_pc[g_x / v]; sc = hi_sc[g_x / v]; }
    return (pc - 1 + sc) / 2;
}

/* count of n in (1, v] with all prime factors > pr[i] and n a sum of two squares */
static uint64_t S(uint64_t v, long long i) {
    uint64_t r = (uint64_t)(fp(v) - fp(pr[i]));
    for (long long k = i + 1; k < npr && pr[k] <= v / pr[k]; k++) {
        uint64_t p = pr[k], pe = p;
        int e = 1;
        int one = (p % 4 == 1);
        while (pe <= v / p) {
            if (one || e % 2 == 0) r += S(v / pe, k);
            if (one || (e + 1) % 2 == 0) r += 1;
            pe *= p;
            e++;
        }
    }
    return r;
}

static uint64_t count74(idx_t k) {
    uint64_t x = (uint64_t)1 << k;
    if (k < 2) return 1;
    uint64_t sq = my_isqrt(x);
    g_x = x; g_sq = sq;
    lo_pc = (int64_t *)malloc((size_t)(sq + 2) * sizeof(int64_t));
    hi_pc = (int64_t *)malloc((size_t)(sq + 2) * sizeof(int64_t));
    lo_sc = (int64_t *)malloc((size_t)(sq + 2) * sizeof(int64_t));
    hi_sc = (int64_t *)malloc((size_t)(sq + 2) * sizeof(int64_t));
    pr = (uint64_t *)malloc((size_t)(sq + 2) * sizeof(uint64_t));
    for (uint64_t v = 1; v <= sq; v++) {
        lo_pc[v] = (int64_t)v - 1;
        lo_sc[v] = (v % 4 == 1 || v % 4 == 2) ? 0 : -1;
    }
    for (uint64_t i = 1; i <= sq; i++) {
        uint64_t v = x / i;
        hi_pc[i] = (int64_t)v - 1;
        hi_sc[i] = (v % 4 == 1 || v % 4 == 2) ? 0 : -1;
    }
    npr = 0;
    for (uint64_t p = 2; p <= sq; p++) {
        if (lo_pc[p] == lo_pc[p - 1]) continue;
        pr[npr++] = p;
        int64_t cp = lo_pc[p - 1], cs = lo_sc[p - 1];
        int ch = (p == 2) ? 0 : (p % 4 == 1 ? 1 : -1);
        uint64_t p2 = p * p;
        for (uint64_t i = 1; i <= sq && x / i >= p2; i++) {
            uint64_t ip = i * p;
            int64_t vp, vs;
            if (ip <= sq) { vp = hi_pc[ip]; vs = hi_sc[ip]; }
            else { uint64_t w = x / ip; vp = lo_pc[w]; vs = lo_sc[w]; }
            hi_pc[i] -= vp - cp;
            hi_sc[i] -= ch * (vs - cs);
        }
        for (uint64_t v = sq; v >= p2; v--) {
            uint64_t w = v / p;
            lo_pc[v] -= lo_pc[w] - cp;
            lo_sc[v] -= ch * (lo_sc[w] - cs);
        }
    }
    /* pr[0] == 2: count odd n > 1, then add n = 1 */
    uint64_t res = 1 + S(x, 0);
    free(lo_pc); free(hi_pc); free(lo_sc); free(hi_sc); free(pr);
    return res;
}

static uint64_t memo74[64];
static int have74[64];

seq_t a(idx_t n) {
    if (n == 1) return 1;
    if (n == 2) return 2;
    seq_t r = (n % 2 == 1) ? (seq_t)1 : (seq_t)2;
    for (idx_t k = n; k >= 3; k -= 2) {
        if (k < 64) {
            if (!have74[k]) { memo74[k] = count74(k); have74[k] = 1; }
            r += (seq_t)memo74[k];
        }
    }
    return r;
}
