/*
 * A229301  Numbers n such that A031971(42*n) == n (mod 42*n).
 * https://oeis.org/A229301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k >= 1 with A031971(42k) == k (mod 42k), where
 *            A031971(m) = Sum_{j=1..m} j^m; found by search, using the
 *            completely multiplicative j^m mod 42k and a
 *            smallest-prime-factor sieve
 * Range:     u8 n <= 203, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 83 terms through n = 985 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint32_t A229301_powmod(uint32_t b, uint32_t e, uint32_t m)
{
    uint64_t r = 1 % m, x = b % m;
    while (e) {
        if (e & 1) r = r * x % m;
        x = x * x % m;
        e >>= 1;
    }
    return (uint32_t)r;
}

static int A229301_test(uint64_t n)
{
    uint32_t m = (uint32_t)(42 * n);
    uint32_t *spf = (uint32_t *)calloc((size_t)m + 1, sizeof(uint32_t));
    uint32_t *f = (uint32_t *)calloc((size_t)m + 1, sizeof(uint32_t));
    uint32_t *pr = (uint32_t *)malloc(sizeof(uint32_t) * ((size_t)m / 2 + 16));
    size_t pc = 0;
    uint64_t sum = 0;
    if (!spf || !f || !pr) { free(spf); free(f); free(pr); return 0; }
    f[1] = 1 % m;
    sum = f[1];
    for (uint32_t k = 2; k <= m; ++k) {
        if (spf[k] == 0) {
            spf[k] = k;
            pr[pc++] = k;
            f[k] = A229301_powmod(k, m, m);
        } else {
            uint32_t p = spf[k];
            f[k] = (uint32_t)((uint64_t)f[p] * f[k / p] % m);
        }
        for (size_t i = 0; i < pc; ++i) {
            uint64_t v = (uint64_t)pr[i] * k;
            if (pr[i] > spf[k] || v > m) break;
            spf[v] = pr[i];
        }
        sum += f[k];
    }
    sum %= m;
    free(spf); free(f); free(pr);
    return sum == (uint64_t)(n % m);
}

seq_t a(idx_t n)
{
    static uint64_t *terms = 0;
    static size_t cnt = 0, cap = 0;
    static uint64_t next = 1;
    if (n == 0) return (seq_t)0;
    while (cnt < (size_t)n) {
        if (A229301_test(next)) {
            if (cnt == cap) {
                cap = cap ? cap * 2 : 1024;
                terms = (uint64_t *)realloc(terms, cap * sizeof(uint64_t));
            }
            terms[cnt++] = next;
        }
        ++next;
    }
    return (seq_t)terms[n - 1];
}
