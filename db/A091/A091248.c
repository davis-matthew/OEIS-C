/*
 * A091248  Number of irreducible factors in the factorization of X^n + 1 over
 *          GF(2) (counted with multiplicity).
 * https://oeis.org/A091248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{d|n} φ(d)/ord_d(2), for odd d; a(2n) = 2·a(n)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

/* modular multiplication (a*b) mod mod, avoiding overflow */
static inline uint64_t mul_mod_uint64(uint64_t a, uint64_t b, uint64_t mod)
{
    uint64_t res = 0;
    a %= mod;
    while (b) {
        if (b & 1) {
            res += a;
            if (res >= mod) res -= mod;
        }
        if (a >= mod) a -= mod;
        a += a;
        if (a >= mod) a -= mod;
        b >>= 1;
    }
    return res;
}

/* modular exponentiation a^exp mod mod */
static inline uint64_t mod_pow_uint64(uint64_t a, uint64_t exp, uint64_t mod)
{
    uint64_t res = 1 % mod;
    a %= mod;
    while (exp) {
        if (exp & 1) res = mul_mod_uint64(res, a, mod);
        exp >>= 1;
        if (exp) a = mul_mod_uint64(a, a, mod);
    }
    return res;
}

/* multiplicative order of a modulo n (gcd(a,n)=1), returns 1 if n==1 */
static uint64_t multiplicative_order(uint64_t a, uint64_t n)
{
    if (n == 1) return 1;
    /* compute φ(n) */
    uint64_t phi = n;
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        phi = phi / p * (p - 1);
    }
    /* factor φ(n) */
    uint64_t ord = phi;
    uint64_t primes_ord[64], exps_ord[64];
    size_t cnt_ord = 0;
    factor_u64(phi, primes_ord, exps_ord, &cnt_ord);
    for (size_t i = 0; i < cnt_ord; ++i) {
        uint64_t p = primes_ord[i];
        while (ord % p == 0 && mod_pow_uint64(a, ord / p, n) == 1)
            ord /= p;
    }
    return ord;
}

/* term φ(d)/ord_d(2) for an odd divisor d */
static seq_t term_for_divisor(uint64_t d)
{
    if (d == 1) return (seq_t)1;
    /* φ(d) */
    uint64_t phi = d;
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64(d, primes, exps, &cnt);
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        phi = phi / p * (p - 1);
    }
    uint64_t ord = multiplicative_order(2, d);
    return (seq_t)(phi / ord);
}

/* depth‑first generation of divisors, accumulating result in *sum */
static void dfs_divisors(const uint64_t *pr, const uint64_t *ex, size_t cnt,
                         size_t idx, uint64_t cur, seq_t *sum)
{
    if (idx == cnt) {
        *sum += term_for_divisor(cur);
        return;
    }
    uint64_t val = 1;
    for (uint64_t i = 0; i <= ex[idx]; ++i) {
        dfs_divisors(pr, ex, cnt, idx + 1, cur * val, sum);
        val *= pr[idx];
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    uint64_t nn = (uint64_t)n;
    unsigned k = 0;
    while ((nn & 1ULL) == 0) {
        nn >>= 1;
        ++k;
    }
    uint64_t odd = nn;                 /* odd part of n */
    seq_t sum = 0;
    if (odd == 1) {
        sum = (seq_t)1;
    } else {
        uint64_t primes[64], exps[64];
        size_t cnt = 0;
        factor_u64(odd, primes, exps, &cnt);
        dfs_divisors(primes, exps, cnt, 0, 1, &sum);
    }
    /* multiply by 2^k */
    for (unsigned i = 0; i < k; ++i) sum *= 2;
    return sum;
}
