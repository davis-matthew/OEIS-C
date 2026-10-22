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
 * Helpers:   a2890_isp, a2890_nprimes, a2890_pw, a2890_run
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/a2890_isp.h"
#include "../utils/a2890_nprimes.h"
#include "../utils/a2890_pw.h"
#include "../utils/a2890_run.h"

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
