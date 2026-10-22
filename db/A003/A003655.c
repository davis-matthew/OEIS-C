/*
 * A003655  Discriminants of real quadratic fields with narrow class number 1.
 * https://oeis.org/A003655
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search. By genus theory, narrow class number 1 forces D = 8 or D
 *            = p, a prime == 1 (mod 4). For each such candidate D, count all
 *            reduced indefinite forms (a,b,c) of discriminant D, then find
 *            the length of the principal cycle under the reduction operator
 *            rho. Narrow h(D) = 1 exactly when that cycl...
 * Range:     u8 n <= 24, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

static uint32_t *spf = NULL;
static uint64_t spf_lim = 0;

static void build_sieve(uint64_t n) {
    free(spf);
    spf = (uint32_t *)calloc(n + 1, sizeof(uint32_t));
    spf_lim = n;
    for (uint64_t i = 2; i <= n; i++) {
        if (spf[i] == 0) {
            for (uint64_t j = i; j <= n; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
}

static uint64_t isqrt64(uint64_t d) {
    uint64_t lo = 0, hi = 1;
    while (hi * hi <= d) hi <<= 1;
    while (hi - lo > 1) {
        uint64_t mid = (lo + hi) / 2;
        if (mid * mid <= d) lo = mid; else hi = mid;
    }
    return lo;
}

static uint64_t divs[4096];

/* number of divisors x of m with lo <= 2x <= hi */
static uint64_t count_divs(uint64_t m, uint64_t lo, uint64_t hi) {
    int cnt = 1;
    divs[0] = 1;
    while (m > 1) {
        uint64_t p = spf[m];
        int e = 0;
        while (m % p == 0) { m /= p; e++; }
        int size = cnt;
        uint64_t mult = 1;
        for (int k = 1; k <= e; k++) {
            mult *= p;
            for (int i = 0; i < size; i++) divs[cnt++] = divs[i] * mult;
        }
    }
    uint64_t r = 0;
    for (int i = 0; i < cnt; i++) {
        uint64_t t = 2 * divs[i];
        if (t >= lo && t <= hi) r++;
    }
    return r;
}

static bool narrow_h1(uint64_t D) {
    uint64_t s = isqrt64(D);
    /* total number of reduced forms */
    uint64_t N = 0;
    uint64_t bstart = (D % 2 == 0) ? 2 : 1;
    for (uint64_t b = bstart; b <= s; b += 2) {
        uint64_t m = (D - b * b) / 4;
        N += count_divs(m, s + 1 - b, s + b);
    }
    N *= 2; /* a may be positive or negative */

    /* principal cycle */
    int64_t Ds = (int64_t)D, ss = (int64_t)s;
    int64_t b0 = (int64_t)((s % 2 == D % 2) ? s : s - 1);
    int64_t a = 1, b = b0, c = (b0 * b0 - Ds) / 4;
    uint64_t L = 0;
    for (;;) {
        int64_t k = 2 * (c < 0 ? -c : c);
        int64_t bn = ss - ((ss + b) % k);
        int64_t cn = (bn * bn - Ds) / (4 * c);
        a = c; b = bn; c = cn;
        L++;
        if (a == 1 && b == b0) break;
        if (L > N) return false;
    }
    return L == N;
}

seq_t a(idx_t n) {
    static uint64_t *cache = NULL;
    static idx_t cache_len = 0, cache_cap = 0;
    static uint64_t next_cand = 5;

    while (cache_len < n) {
        uint64_t d = next_cand++;
        if (d >= spf_lim) build_sieve(2 * d + 1024);
        bool cand = (d == 8) || (d % 4 == 1 && spf[d] == d);
        if (!cand) continue;
        if (narrow_h1(d)) {
            if (cache_len == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 256;
                cache = (uint64_t *)realloc(cache, cache_cap * sizeof(uint64_t));
            }
            cache[cache_len++] = d;
        }
    }
    return (seq_t)cache[n - 1];
}
