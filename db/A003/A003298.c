/*
 * A003298  Denominators of coefficients of Green function for cubic lattice.
 * https://oeis.org/A003298
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = denominator of f(n), where f(0) = 1, f(1) = 1/18 and
 *            36*n*(n+1)*(2n+1)*f(n+1) = 4n*(20n^2+1)*f(n) - (2n-1)^3*f(n-1).
 *            With D(n) = 2^(n-1)*9^n*(n-1)!*(2n)!, G(n) = D(n)*f(n) is an
 *            integer: G(1) = 1, G(2) = 66, G(m+1) = 4m(20m^2+1)G(m) -
 *            36m(m-1)(2m-1)^4 G(m-1), and a(n) = D(n)/gcd(D(n), G(n))
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 13,
 *            big any n
 * Helpers:   g_div, g_mod, g_mulsm, g_spow, g_vG, g_vfact
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/g_div.h"
#include "../utils/g_mod.h"
#include "../utils/g_mulsm.h"
#include "../utils/g_spow.h"
#include "../utils/g_vG.h"
#include "../utils/g_vfact.h"

static uint64_t *gb[4];
static size_t glen[4];
static size_t gcap = 0;
static idx_t gcur = 0;

static void g_ensure(size_t need)
{
    int i;
    size_t nc;
    if (need <= gcap) return;
    nc = need * 2 + 16;
    for (i = 0; i < 4; i++)
        gb[i] = (uint64_t *)realloc(gb[i], nc * sizeof(uint64_t));
    gcap = nc;
}

static void g_reset(void)
{
    g_ensure(8);
    gb[0][0] = 1; glen[0] = 1;
    gb[1][0] = 66; glen[1] = 1;
    gcur = 2;
}

static void g_step(void)
{
    uint64_t m = (uint64_t)gcur, q = 2 * m - 1;
    uint64_t *t, *u, *old0;
    size_t lt, lu, i;
    uint64_t borrow = 0;
    g_ensure(glen[1] + 6);
    t = gb[2]; u = gb[3];
    memcpy(t, gb[1], glen[1] * sizeof(uint64_t)); lt = glen[1];
    g_mulsm(t, &lt, 4 * m * (20 * m * m + 1));
    memcpy(u, gb[0], glen[0] * sizeof(uint64_t)); lu = glen[0];
    g_mulsm(u, &lu, 36 * m * (m - 1));
    g_mulsm(u, &lu, q * q);
    g_mulsm(u, &lu, q * q);
    for (i = 0; i < lt; i++) {
        uint64_t s = (i < lu) ? u[i] : 0;
        uint64_t x = t[i];
        uint64_t d = x - s - borrow;
        borrow = (x < s || (x - s) < borrow) ? 1 : 0;
        t[i] = d;
    }
    while (lt > 1 && t[lt - 1] == 0) lt--;
    old0 = gb[0];
    gb[0] = gb[1]; glen[0] = glen[1];
    gb[1] = t; glen[1] = lt;
    gb[2] = old0;
    gcur++;
}

seq_t a(idx_t n)
{
    const uint64_t *G;
    size_t len, i, np = 0;
    uint64_t N, *w, *primes;
    unsigned char *comp;
    seq_t res = 1;
    long long e, v;

    if (n == 0) return 1;
    if (gcap == 0 || n + 1 < gcur) g_reset();
    while (gcur < n) g_step();
    if (n == gcur) { G = gb[1]; len = glen[1]; }
    else { G = gb[0]; len = glen[0]; }

    w = (uint64_t *)malloc((len + 1) * sizeof(uint64_t));

    /* p = 2 */
    e = (long long)(n - 1) + g_vfact(n - 1, 2) + g_vfact(2 * n, 2);
    {
        long long tz = 0;
        size_t k = 0;
        while (k < len && G[k] == 0) { tz += 64; k++; }
        if (k < len) {
            uint64_t x = G[k];
            while (!(x & 1)) { x >>= 1; tz++; }
        }
        v = tz < e ? tz : e;
        res *= g_spow((seq_t)2, e - v);
    }

    /* odd primes up to max(2n, 3): 3 always divides D(n) */
    N = 2 * (uint64_t)n;
    if (N < 3) N = 3;
    comp = (unsigned char *)calloc((size_t)N + 2, 1);
    primes = (uint64_t *)malloc(((size_t)N + 2) * sizeof(uint64_t));
    for (i = 3; i <= N; i += 2) {
        if (!comp[i]) {
            size_t j;
            primes[np++] = i;
            for (j = i * i; j <= N; j += 2 * i) comp[j] = 1;
        }
    }

    for (i = 0; i < np; i += 2) {
        uint64_t p1 = primes[i];
        uint64_t p2 = (i + 1 < np) ? primes[i + 1] : 1;
        uint64_t r = g_mod(G, len, p1 * p2);
        int k;
        for (k = 0; k < 2; k++) {
            uint64_t p = k == 0 ? p1 : p2;
            if (p == 1) continue;
            e = g_vfact(n - 1, p) + g_vfact(2 * n, p);
            if (p == 3) e += 2 + 2 * (long long)(n - 1);
            if (e <= 0) continue;
            v = 0;
            if (r % p == 0) v = g_vG(G, len, w, p, e);
            res *= g_spow((seq_t)p, e - v);
        }
    }

    free(comp);
    free(primes);
    free(w);
    return res;
}
