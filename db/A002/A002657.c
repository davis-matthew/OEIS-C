/*
 * A002657  Numerators of Cauchy numbers of second type (= Bernoulli numbers
 *          B_n^{(n)}).
 * https://oeis.org/A002657
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy, nice
 * Formula:   a(n) = numerator of Sum_{k=0..n} |Stirling1(n,k)|/(k+1) (=
 *            integral of x(x+1)...(x+n-1) from 0 to 1); computed as N = Sum_k
 *            |s(n,k)| * (L/(k+1)), L = lcm(1..n+1), a(n) = N / gcd(N, L)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   a001192_zalloc, a2657_fixed, a2657_sieve
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../utils/a001192_zalloc.h"
#include "../utils/a2657_fixed.h"
#include "../utils/a2657_sieve.h"

#define A2657_LW 16

static void a2657_muls(uint32_t *x, uint32_t m) {
    uint64_t c = 0;
    int i;
    for (i = 0; i < A2657_LW; i++) {
        uint64_t t = (uint64_t)x[i] * m + c;
        x[i] = (uint32_t)t;
        c = t >> 32;
    }
}

static void a2657_add(uint32_t *x, const uint32_t *y) {
    uint64_t c = 0;
    int i;
    for (i = 0; i < A2657_LW; i++) {
        uint64_t t = (uint64_t)x[i] + y[i] + c;
        x[i] = (uint32_t)t;
        c = t >> 32;
    }
}

static uint32_t a2657_divs(uint32_t *x, uint32_t d) {
    uint64_t r = 0;
    int i;
    for (i = A2657_LW - 1; i >= 0; i--) {
        uint64_t cur = (r << 32) | x[i];
        x[i] = (uint32_t)(cur / d);
        r = cur % d;
    }
    return (uint32_t)r;
}

static void a2657_muladd(uint32_t *acc, const uint32_t *x, const uint32_t *y) {
    int i, j;
    for (i = 0; i < A2657_LW; i++) {
        uint64_t c = 0;
        if (x[i] == 0) continue;
        for (j = 0; i + j < A2657_LW; j++) {
            uint64_t t = (uint64_t)x[i] * y[j] + acc[i + j] + c;
            acc[i + j] = (uint32_t)t;
            c = t >> 32;
        }
    }
}

static seq_t a2657_fixed_calc(idx_t n) {
    uint32_t (*s)[A2657_LW];
    uint32_t L[A2657_LW], N[A2657_LW], q[A2657_LW], t[A2657_LW];
    unsigned char *comp;
    idx_t m, k, p;
    seq_t r = 0;
    int i;

    if (n > 60) return 0; /* cannot fit in any fixed-width seq_t */
    s = (uint32_t (*)[A2657_LW])calloc((size_t)n + 1, sizeof(*s));
    s[0][0] = 1;
    for (m = 0; m < n; m++) {
        for (k = m + 1; k >= 1; k--) {
            a2657_muls(s[k], (uint32_t)m);
            a2657_add(s[k], s[k - 1]);
        }
        a2657_muls(s[0], (uint32_t)m);
    }

    comp = a2657_sieve(n + 1);
    memset(L, 0, sizeof(L));
    L[0] = 1;
    for (p = 2; p <= n + 1; p++) {
        idx_t pk;
        if (comp[p]) continue;
        for (pk = p; pk <= n + 1; pk *= p) a2657_muls(L, (uint32_t)p);
    }

    memset(N, 0, sizeof(N));
    for (k = 0; k <= n; k++) {
        memcpy(q, L, sizeof(L));
        a2657_divs(q, (uint32_t)(k + 1));
        a2657_muladd(N, s[k], q);
    }

    for (p = 2; p <= n + 1; p++) {
        idx_t pk;
        if (comp[p]) continue;
        for (pk = p; pk <= n + 1; pk *= p) {
            memcpy(t, N, sizeof(N));
            if (a2657_divs(t, (uint32_t)p) != 0) break;
            memcpy(N, t, sizeof(N));
        }
    }

    for (i = A2657_LW - 1; i >= 0; i--) {
        r = r * (idx_t)65536;
        r = r * (idx_t)65536;
        r += (idx_t)N[i];
    }
    free(comp);
    free(s);
    return r;
}

static seq_t a2657_big_calc(idx_t n) {
    seq_t *s = a001192_zalloc(n + 2);
    unsigned char *comp;
    seq_t L = 1, N = 0;
    idx_t m, k, p;

    s[0] = 1;
    for (m = 0; m < n; m++) {
        for (k = m + 1; k >= 1; k--)
            s[k] = s[k] * m + s[k - 1];
        s[0] = s[0] * m;
    }

    comp = a2657_sieve(n + 1);
    for (p = 2; p <= n + 1; p++) {
        idx_t pk;
        if (comp[p]) continue;
        for (pk = p; pk <= n + 1; pk *= p) L = L * p;
    }

    for (k = 0; k <= n; k++)
        N += s[k] * (L / (k + 1));

    for (p = 2; p <= n + 1; p++) {
        idx_t pk;
        if (comp[p]) continue;
        for (pk = p; pk <= n + 1; pk *= p) {
            if (N % p != 0) break;
            N = N / p;
        }
    }

    free(comp);
    free(s);
    return N;
}

seq_t a(idx_t n) {
    if (a2657_fixed()) return a2657_fixed_calc(n);
    return a2657_big_calc(n);
}
