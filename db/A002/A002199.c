/*
 * A002199  Least negative primitive root of n-th prime.
 * https://oeis.org/A002199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n) - A071894(n); computed by search: least k >= 1
 *            such that prime(n) - k is a primitive root mod prime(n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a2199_powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a2199_powmod.h"

static uint64_t *a2199_primes = NULL;
static uint64_t a2199_count = 0;

static void a2199_sieve(uint64_t n) {
    uint64_t limit = 64;
    while (1) {
        bool *comp = (bool *)calloc((size_t)(limit + 1), sizeof(bool));
        uint64_t cnt = 0, i, j;
        for (i = 2; i <= limit; i++) {
            if (!comp[i]) {
                cnt++;
                for (j = i * i; j <= limit; j += i) comp[j] = true;
            }
        }
        if (cnt >= n) {
            free(a2199_primes);
            a2199_primes = (uint64_t *)malloc((size_t)cnt * sizeof(uint64_t));
            a2199_count = 0;
            for (i = 2; i <= limit; i++)
                if (!comp[i]) a2199_primes[a2199_count++] = i;
            free(comp);
            return;
        }
        free(comp);
        limit *= 2;
    }
}

seq_t a(idx_t n) {
    uint64_t p, phi, m, d, k;
    uint64_t fac[64];
    int nf = 0, i;
    if (n > a2199_count) a2199_sieve((uint64_t)n);
    p = a2199_primes[n - 1];
    phi = p - 1;
    m = phi;
    for (d = 2; d * d <= m; d++) {
        if (m % d == 0) {
            fac[nf++] = d;
            while (m % d == 0) m /= d;
        }
    }
    if (m > 1) fac[nf++] = m;
    for (k = 1; k < p; k++) {
        uint64_t g = p - k;
        bool ok = true;
        for (i = 0; i < nf; i++) {
            if (a2199_powmod(g, phi / fac[i], p) == 1) { ok = false; break; }
        }
        if (ok) return (seq_t)k;
    }
    return (seq_t)0;
}
