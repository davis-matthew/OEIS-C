/*
 * A002890  Low temperature series for spin-1/2 Ising partition function on 2D
 *          square lattice.
 * https://oeis.org/A002890
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   log(Sum_{n>=0} a(n)*u^n) = log(1+u) - Sum_{j>=1}
 *            binomial(2j,j)^2 * v^j / (4j), where v = u*(1-u)^2/(1+u)^4. This
 *            comes from Onsager's solution with u = exp(-4K). The series is
 *            computed modulo several primes: the inner sums use g_{k+1} =
 *            (-6j*g_k + (2j+k-1)*g_{k-1})/(k+1), giving the coefficients...
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 18, u64 n <= 32, u128 n <= 58,
 *            big any n
 * Validated: standard, pass, 41 terms through n = 474 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static int a2890_isp(uint64_t x) {
    uint64_t d;
    if (x % 2 == 0) return 0;
    for (d = 3; d * d <= x; d += 2)
        if (x % d == 0) return 0;
    return 1;
}

static int a2890_nprimes(idx_t n, int w) {
    uint64_t bits = 2544 * (uint64_t)n / 1000 + 40;
    if (w < 300 && bits > (uint64_t)w + 40) bits = (uint64_t)w + 40;
    return (int)(bits / 30 + 1);
}

static uint64_t a2890_pw(uint64_t b, uint64_t e, uint64_t p) {
    uint64_t r = 1 % p;
    b %= p;
    while (e) {
        if (e & 1) r = r * b % p;
        b = b * b % p;
        e >>= 1;
    }
    return r;
}

static void a2890_run(uint64_t p, idx_t N, uint32_t *out, uint64_t *inv, uint64_t *l) {
    idx_t k, j, n;
    uint64_t inv4, C;
    inv[1] = 1;
    for (k = 2; k <= N; k++) inv[k] = (p - (p / k) * inv[p % k] % p) % p;
    l[0] = 0;
    for (k = 1; k < N; k++) l[k] = (k & 1) ? inv[k] : (p - inv[k]) % p;
    inv4 = a2890_pw(4, p - 2, p);
    C = 1;
    for (j = 1; j < N; j++) {
        uint64_t coef, gp = 0, gc = 1, m6;
        C = C * ((2 * (2 * j - 1)) % p) % p * inv[j] % p;
        coef = C * C % p * inv[j] % p * inv4 % p;
        m6 = (p - (6 * j) % p) % p;
        for (k = 0; j + k < N; k++) {
            uint64_t nx;
            l[j + k] = (l[j + k] + p - coef * gc % p) % p;
            nx = (m6 * gc + ((2 * j + k - 1) % p) * gp) % p;
            nx = nx * inv[k + 1] % p;
            gp = gc;
            gc = nx;
        }
    }
    for (k = 1; k < N; k++) l[k] = l[k] * (k % p) % p;
    out[0] = (uint32_t)(1 % p);
    for (n = 1; n < N; n++) {
        uint64_t s = 0;
        for (k = 1; k <= n; k++) s = (s + l[k] * (uint64_t)out[n - k]) % p;
        out[n] = (uint32_t)(s * inv[n] % p);
    }
}

static idx_t g_cap = 0;
static int g_np = 0;
static uint64_t *g_pr = NULL;
static uint32_t *g_res = NULL;
static int g_w = -1;

static int a2890_width(void) {
    if (g_w < 0) {
        seq_t x = 1;
        int w = 0;
        while (x != 0 && w < 300) { x <<= 1; w++; }
        g_w = w;
    }
    return g_w;
}

static void a2890_rebuild(idx_t N, int np) {
    uint64_t c = 2147483647ULL;
    uint64_t *inv, *l;
    int i;
    free(g_res);
    free(g_pr);
    g_pr = (uint64_t *)malloc((size_t)np * sizeof(uint64_t));
    for (i = 0; i < np; i++) {
        while (!a2890_isp(c)) c -= 2;
        g_pr[i] = c;
        c -= 2;
    }
    g_res = (uint32_t *)malloc((size_t)np * (size_t)N * sizeof(uint32_t));
    inv = (uint64_t *)malloc((size_t)(N + 2) * sizeof(uint64_t));
    l = (uint64_t *)malloc((size_t)(N + 2) * sizeof(uint64_t));
    for (i = 0; i < np; i++)
        a2890_run(g_pr[i], N, g_res + (size_t)i * (size_t)N, inv, l);
    free(inv);
    free(l);
    g_cap = N;
    g_np = np;
}

seq_t a(idx_t n) {
    int w = a2890_width();
    int need, i, k;
    uint64_t *d;
    if (w < 300 && n > (idx_t)w + 8) return 0; /* cannot fit in seq_t */
    need = a2890_nprimes(n, w);
    if (n >= g_cap || need > g_np) {
        idx_t N = g_cap;
        int np;
        if (n >= N) N = n + 1 + n / 4 + 8;
        np = a2890_nprimes(N - 1, w);
        if (np < need) np = need;
        if (np < g_np) np = g_np;
        a2890_rebuild(N, np);
    }
    d = (uint64_t *)malloc((size_t)need * sizeof(uint64_t));
    for (i = 0; i < need; i++) {
        uint64_t p = g_pr[i];
        uint64_t t = (uint64_t)g_res[(size_t)i * (size_t)g_cap + n] % p;
        for (k = 0; k < i; k++)
            t = (t + p - d[k] % p) % p * a2890_pw(g_pr[k] % p, p - 2, p) % p;
        d[i] = t;
    }
    if (w <= 64) {
        uint64_t x = 0;
        for (i = need; i-- > 0;) x = x * g_pr[i] + d[i];
        free(d);
        return (seq_t)x;
    } else {
        seq_t x = 0;
        for (i = need; i-- > 0;) x = x * (seq_t)g_pr[i] + (seq_t)d[i];
        free(d);
        return x;
    }
}
