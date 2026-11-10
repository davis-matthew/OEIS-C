/*
 * A226305  Numerator of Product_{d|n} b(d)^Moebius(n/d), where b() =
 *          A100371().
 * https://oeis.org/A226305
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of Product_{d|n} (2^phi(d)-1)^mu(n/d) * =
 *            numerator of Product_{k>=2} Phi_k(2)^E_k, where Phi_k is the
 *            k-th cyclotomic polynomial and * E_k = Sum_{d|n, k|phi(d)}
 *            mu(n/d). Common factors between different Phi_k(2) are only the
 *            * primes p with k = ord_p(2)*p^j. Those primes are spl...
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 36, u64 n <= 66, u128 n <= 130,
 *            big any n
 * Validated: standard, pass, 68 terms through n = 996 (2026-11-07)
 * Helpers:   largest_prime_factor, mobius_u64, powmod_u64, totient_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/totient_u64.h"
#include "../utils/mobius_u64.h"
#include "../utils/largest_prime_factor.h"
#include "../utils/powmod_u64.h"

static int A226305_is_big(void)
{
    seq_t x = 1;
    for (int i = 0; i < 140; i++) x += x;
    return x != (seq_t)0;
}

static seq_t A226305_inv_odd(seq_t y)
{
    seq_t x = y;
    for (int i = 0; i < 8; i++) {
        seq_t t = y * x;
        t = (seq_t)2 - t;
        x = x * t;
    }
    return x;
}

static seq_t A226305_divx(seq_t x, seq_t y, int big)
{
    if (big) return x / y;
    return x * A226305_inv_odd(y);
}

static seq_t A226305_pow2w(uint64_t d)
{
    seq_t r = 1, b = 2;
    while (d) {
        if (d & 1) r = r * b;
        d >>= 1;
        if (d) b = b * b;
    }
    return r;
}

/* Phi_k(2), exact (big) or modulo 2^w (fixed width) */
static seq_t A226305_phi2(uint64_t k, int big)
{
    seq_t pos = 1, neg = 1;
    for (uint64_t d = 1; d <= k; d++) {
        if (k % d) continue;
        int mu = mobius_u64(k / d);
        if (mu == 1) pos = pos * (A226305_pow2w(d) - (seq_t)1);
        else if (mu == -1) neg = neg * (A226305_pow2w(d) - (seq_t)1);
    }
    return A226305_divx(pos, neg, big);
}

static uint64_t A226305_ord2(uint64_t p)
{
    uint64_t t = 1, x = 2 % p;
    while (x != 1) { x = (x * 2) % p; t++; }
    return t;
}

seq_t a(idx_t n)
{
    int big = A226305_is_big();
    uint64_t N = (uint64_t)n;
    uint64_t M = totient_u64(N);
    if (M < 1) M = 1;

    long long *e = (long long *)calloc(M + 1, sizeof(long long));
    long long *E = (long long *)calloc(M + 1, sizeof(long long));
    uint64_t *rp = (uint64_t *)calloc(M + 1, sizeof(uint64_t));
    uint64_t *ro = (uint64_t *)calloc(M + 1, sizeof(uint64_t));
    int *rv = (int *)calloc(M + 1, sizeof(int));
    long long *rf = (long long *)calloc(M + 1, sizeof(long long));
    size_t nr = 0;

    for (uint64_t d = 1; d <= N; d++) {
        if (N % d) continue;
        int mu = mobius_u64(N / d);
        if (mu) e[totient_u64(d)] += mu;
    }
    for (uint64_t k = 2; k <= M; k++) {
        long long s = 0;
        for (uint64_t m = k; m <= M; m += k) s += e[m];
        E[k] = s;
    }

    /* non-primitive primes */
    for (uint64_t k = 2; k <= M; k++) {
        if (!E[k]) continue;
        uint64_t p = largest_prime_factor(k);
        if (p == 2) continue;
        uint64_t q = k;
        while (q % p == 0) q /= p;
        if ((p - 1) % q != 0) continue;
        if (A226305_ord2(p) != q) continue;
        int found = 0;
        for (size_t i = 0; i < nr; i++) if (rp[i] == p) { found = 1; break; }
        if (!found) { rp[nr] = p; ro[nr] = q; nr++; }
    }
    for (size_t i = 0; i < nr; i++) {
        uint64_t p = rp[i], o = ro[i];
        int v = 1;
        uint64_t pk = p * p;
        while (pk <= ((uint64_t)1 << 62) / p + 1 && powmod_u64(2, o, pk) == 1) {
            v++;
            pk *= p;
        }
        rv[i] = v;
        long long f = (long long)v * E[o];
        for (uint64_t k = o * p; k <= M; k *= p) {
            f += E[k];
            if (k > M / p) break;
        }
        rf[i] = f;
    }

    seq_t num = 1;
    for (uint64_t k = 2; k <= M; k++) {
        if (E[k] <= 0) continue;
        seq_t P = A226305_phi2(k, big);
        for (size_t i = 0; i < nr; i++) {
            uint64_t p = rp[i], o = ro[i];
            if (k == o) {
                for (int t = 0; t < rv[i]; t++) P = A226305_divx(P, (seq_t)p, big);
            } else if (k % o == 0) {
                uint64_t r = k / o;
                if (r > 1) {
                    while (r % p == 0) r /= p;
                    if (r == 1) P = A226305_divx(P, (seq_t)p, big);
                }
            }
        }
        for (long long t = 0; t < E[k]; t++) num = num * P;
    }
    for (size_t i = 0; i < nr; i++)
        for (long long t = 0; t < rf[i]; t++) num = num * (seq_t)rp[i];

    free(e); free(E); free(rp); free(ro); free(rv); free(rf);
    return num;
}
