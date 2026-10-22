/*
 * A002427  Numerator of (2n+1) B_{2n}, where B_n are the Bernoulli numbers.
 * https://oeis.org/A002427
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, nice, frac
 * Formula:   a(n) = numerator((2n+1)*B_{2n}) = N*(2n+1)/gcd(2n+1,D), where
 *            B_{2n} = N/D and D = prod of primes q with (q-1) | 2n (von
 *            Staudt-Clausen). With m = 2n, a(n) is computed modulo many
 *            primes p with (p-1) not dividing m, using Voronoi's congruence
 *            (2^m - 1)*B_m/m == -2^(m-1) * Sum_{k=1..(p-1)/2} k^((m...
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 13, i64 n <= 17, i128 n <= 29,
 *            big any n
 * Helpers:   a2427_isprime, a2427_powm
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/a2427_isprime.h"
#include "../utils/a2427_powm.h"

static uint32_t *a2427_spf = NULL;
static uint64_t a2427_spf_lim = 0;

static void a2427_sieve(uint64_t L) {
    if (L <= a2427_spf_lim) return;
    uint64_t nl = a2427_spf_lim ? a2427_spf_lim : 1024;
    while (nl < L) nl *= 2;
    free(a2427_spf);
    a2427_spf = (uint32_t *)calloc((size_t)nl + 1, sizeof(uint32_t));
    for (uint64_t i = 2; i <= nl; i++)
        if (a2427_spf[i] == 0)
            for (uint64_t j = i; j <= nl; j += i)
                if (a2427_spf[j] == 0) a2427_spf[j] = (uint32_t)i;
    a2427_spf_lim = nl;
}

seq_t a(idx_t n) {
    if (n == 0) return 1;
    uint64_t m = 2 * (uint64_t)n;

    /* primes of the denominator D, the factor f = (m+1)/gcd(m+1,D), and a size bound */
    uint64_t *dq = NULL;
    size_t nd = 0, capd = 0;
    uint64_t f = m + 1;
    double lg = 2.0 + log2((double)(m + 1)) - (double)m * log2(2.0 * 3.14159265358979323846);
    for (uint64_t k = 2; k <= m; k++) lg += log2((double)k);
    for (uint64_t d = 1; d <= m; d++) {
        if (m % d != 0 || !a2427_isprime(d + 1)) continue;
        if (nd == capd) {
            capd = capd ? 2 * capd : 16;
            dq = (uint64_t *)realloc(dq, capd * sizeof(uint64_t));
        }
        dq[nd++] = d + 1;
        lg += log2((double)(d + 1));
        if (f % (d + 1) == 0) f /= (d + 1);
    }
    double need = lg + 8.0;

    /* residues of a(n) modulo suitable primes */
    uint64_t *ps = NULL, *rs = NULL;
    size_t K = 0, cap = 0;
    uint32_t *pw = NULL;
    uint64_t pwcap = 0;
    double have = 0.0;
    for (uint64_t p = 5; have < need; p += 2) {
        a2427_sieve(p);
        if (a2427_spf[p] != p) continue;
        if (m % (p - 1) == 0) continue;
        uint64_t t2 = a2427_powm(2, m, p);
        if (t2 == 1) continue;
        uint64_t h = (p - 1) / 2, e = (m - 1) % (p - 1);
        if (h + 1 > pwcap) {
            pwcap = 2 * (h + 1);
            pw = (uint32_t *)realloc(pw, (size_t)pwcap * sizeof(uint32_t));
        }
        uint64_t T = 1;
        pw[1] = 1;
        for (uint64_t k = 2; k <= h; k++) {
            uint64_t q = a2427_spf[k];
            uint64_t v = (q == k) ? a2427_powm(k, e, p)
                                  : (uint64_t)pw[q] * pw[k / q] % p;
            pw[k] = (uint32_t)v;
            T += v;
        }
        T %= p;
        uint64_t r = (p - T) % p;
        r = r * t2 % p * ((p + 1) / 2) % p;
        r = r * a2427_powm((t2 + p - 1) % p, p - 2, p) % p;
        r = r * (m % p) % p;
        for (size_t i = 0; i < nd; i++) r = r * (dq[i] % p) % p;
        r = r * (f % p) % p;
        if (K == cap) {
            cap = cap ? 2 * cap : 64;
            ps = (uint64_t *)realloc(ps, cap * sizeof(uint64_t));
            rs = (uint64_t *)realloc(rs, cap * sizeof(uint64_t));
        }
        ps[K] = p;
        rs[K] = r;
        K++;
        have += log2((double)p);
    }

    /* balanced mixed-radix digits (Garner) */
    long long *c = (long long *)malloc(K * sizeof(long long));
    for (size_t k = 0; k < K; k++) {
        uint64_t pk = ps[k], acc = 0, prod = 1;
        for (size_t j = 0; j < k; j++) {
            uint64_t cm = c[j] >= 0 ? (uint64_t)c[j] % pk
                                    : (pk - (uint64_t)(-c[j]) % pk) % pk;
            acc = (acc + cm * prod) % pk;
            prod = prod * (ps[j] % pk) % pk;
        }
        uint64_t v = (rs[k] + pk - acc) % pk * a2427_powm(prod, pk - 2, pk) % pk;
        c[k] = v > pk / 2 ? (long long)v - (long long)pk : (long long)v;
    }

    /* Horner from the top digit: intermediates never exceed |a(n)| by much */
    seq_t x = 0;
    for (size_t k = K; k-- > 0; ) {
        if (x != 0) x = x * (seq_t)(long long)ps[k];
        x += (seq_t)c[k];
    }

    free(c);
    free(ps);
    free(rs);
    free(pw);
    free(dq);
    return x;
}
