/*
 * A004194  Number of partitions of 1/n into 3 reciprocals of positive
 *          integers.
 * https://oeis.org/A004194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search. For each x with n < x <= 3n, write 1/n - 1/x = p/q in
 *            lowest terms. Then 1/y + 1/z = p/q with x <= y <= z is
 *            equivalent to (p*y - q)(p*z - q) = q^2. So the code counts
 *            divisors d of q^2 with d <= q, d == -q (mod p), and y = (d+q)/p
 *            >= x
 * Range:     u8 n <= 19, u16 n <= 2519, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint32_t *spf = 0;
static uint64_t spf_len = 0;

static void ensure_spf(uint64_t m) {
    uint64_t i, j;
    if (m < spf_len) return;
    free(spf);
    spf_len = m + 1;
    spf = (uint32_t *)calloc(spf_len, sizeof(uint32_t));
    for (i = 2; i < spf_len; i++) {
        if (spf[i] == 0) {
            for (j = i; j < spf_len; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
}

static uint64_t gcd_u64(uint64_t a, uint64_t b) {
    while (b) { uint64_t t = a % b; a = b; b = t; }
    return a;
}

static int np;
static uint64_t pr[64];
static int ex[64];

static void add_factors(uint64_t v, int sign) {
    while (v > 1) {
        uint64_t p = spf[v];
        int c = 0, k;
        while (v % p == 0) { v /= p; c++; }
        for (k = 0; k < np; k++) if (pr[k] == p) break;
        if (k == np) { pr[np] = p; ex[np] = 0; np++; }
        ex[k] += sign * c;
    }
}

static uint64_t g_q, g_p, g_res, g_x;
static unsigned long long g_cnt;

static void dfs(int k, uint64_t d) {
    int e;
    if (k == np) {
        if (d % g_p == g_res) {
            uint64_t y = (d + g_q) / g_p;
            if (y >= g_x) g_cnt++;
        }
        return;
    }
    for (e = 0; e <= 2 * ex[k]; e++) {
        dfs(k + 1, d);
        if (e < 2 * ex[k]) {
            if (d > g_q / pr[k]) break;
            d *= pr[k];
        }
    }
}

seq_t a(idx_t n) {
    uint64_t N = (uint64_t)n, x;
    g_cnt = 0;
    ensure_spf(3 * N + 1);
    for (x = N + 1; x <= 3 * N; x++) {
        uint64_t p = x - N;
        uint64_t q = N * x;
        uint64_t g = gcd_u64(p, q);
        int k, m;
        p /= g; q /= g;
        np = 0;
        add_factors(N, 1);
        add_factors(x, 1);
        add_factors(g, -1);
        m = 0;
        for (k = 0; k < np; k++) {
            if (ex[k] > 0) { pr[m] = pr[k]; ex[m] = ex[k]; m++; }
        }
        np = m;
        g_q = q; g_p = p; g_x = x;
        g_res = (p - q % p) % p;
        dfs(0, 1);
    }
    return (seq_t)g_cnt;
}
