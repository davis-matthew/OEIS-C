/*
 * A215196  Numbers k such that A143615(k) = k.
 * https://oeis.org/A215196
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n-th k with A143615(k) = k, where A143615(k) =
 *            Sum_{m=1..k, gcd(m,k)=1} tau(m); found by incremental search
 *            using a tau sieve and inclusion-exclusion over squarefree
 *            divisors of k
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Validated: partial, pass, 7 terms through n = 7 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static uint32_t *a215196_tau = NULL;
static uint64_t a215196_tau_n = 0;
static uint64_t a215196_searched = 0;
static uint64_t *a215196_found = NULL;
static size_t a215196_cnt = 0, a215196_cap = 0;

static void a215196_build(uint64_t N)
{
    uint32_t *t = (uint32_t *)malloc((size_t)(N + 1) * sizeof(uint32_t));
    if (!t) return;
    for (uint64_t i = 0; i <= N; ++i) t[i] = 0;
    for (uint64_t i = 1; i <= N; ++i)
        for (uint64_t j = i; j <= N; j += i) t[j]++;
    free(a215196_tau);
    a215196_tau = t;
    a215196_tau_n = N;
}

static uint64_t a215196_f(uint64_t k)
{
    uint64_t primes[32];
    int pc = 0;
    uint64_t x = k;
    for (uint64_t p = 2; p * p <= x; p += (p == 2 ? 1 : 2)) {
        if (x % p == 0) {
            primes[pc++] = p;
            while (x % p == 0) x /= p;
        }
    }
    if (x > 1) primes[pc++] = x;
    uint64_t pos = 0, neg = 0;
    for (uint32_t mask = 0; mask < (1u << pc); ++mask) {
        uint64_t d = 1;
        int bits = 0;
        for (int i = 0; i < pc; ++i)
            if (mask & (1u << i)) { d *= primes[i]; bits++; }
        uint64_t s = 0;
        for (uint64_t t = d; t <= k; t += d) s += a215196_tau[t];
        if (bits & 1) neg += s; else pos += s;
    }
    return pos - neg;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    while (a215196_cnt < (size_t)n) {
        uint64_t L = a215196_tau_n * 2;
        if (L < 20000) L = 20000;
        if (L > a215196_tau_n) a215196_build(L);
        if (!a215196_tau || a215196_tau_n < L) return 0;
        for (uint64_t k = a215196_searched + 1; k <= a215196_tau_n && a215196_cnt < (size_t)n; ++k) {
            a215196_searched = k;
            if (a215196_f(k) == k) {
                if (a215196_cnt == a215196_cap) {
                    size_t nc = a215196_cap ? a215196_cap * 2 : 16;
                    uint64_t *nf = (uint64_t *)realloc(a215196_found, nc * sizeof(uint64_t));
                    if (!nf) return 0;
                    a215196_found = nf;
                    a215196_cap = nc;
                }
                a215196_found[a215196_cnt++] = k;
            }
        }
    }
    return (seq_t)a215196_found[n - 1];
}
