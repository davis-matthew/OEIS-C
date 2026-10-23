/*
 * A129251  Number of distinct prime factors p of n such that p^p is a divisor
 *          of n.
 * https://oeis.org/A129251
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = #{ p prime : p^p | n } (add 1 for each distinct p with
 *            exponent ≥ p)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   u64_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ---- integer helpers ---- */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/u64_gcd.h"

/* multiplication modulo n using 128‑bit arithmetic */
static inline uint64_t mul_mod(uint64_t a, uint64_t b, uint64_t mod) {
    return (uint64_t)(((__uint128_t)a * b) % mod);
}

/* binary exponentiation modulo n */
static inline uint64_t pow_mod(uint64_t a, uint64_t e, uint64_t mod) {
    uint64_t r = 1 % mod;
    while (e) {
        if (e & 1U) r = mul_mod(r, a, mod);
        a = mul_mod(a, a, mod);
        e >>= 1U;
    }
    return r;
}

/* greatest common divisor for 64‑bit unsigned */

/* deterministic Miller–Rabin primality test for 64‑bit integers */
static bool is_prime(uint64_t n) {
    if (n < 2) return false;
    static const uint64_t small_primes[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    size_t i;
    for (i = 0; i < sizeof small_primes / sizeof *small_primes; ++i) {
        uint64_t p = small_primes[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1U) == 0U) { d >>= 1U; ++s; }
    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };
    for (i = 0; i < sizeof bases / sizeof *bases; ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0U) continue;
        uint64_t x = pow_mod(a, d, n);
        if (x == 1U || x == n - 1) continue;
        bool cont = false;
        int r;
        for (r = 1; r < s; ++r) {
            x = mul_mod(x, x, n);
            if (x == n - 1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Pollard–Rho factorisation */
static uint64_t pollard_rho(uint64_t n) {
    if ((n & 1ULL) == 0ULL) return 2;
    if (n % 3ULL == 0ULL) return 3;
    uint64_t c = 1;
    while (1) {
        uint64_t x = 2, y = 2, d = 1;
        while (d == 1) {
            x = (mul_mod(x, x, n) + c) % n;
            y = (mul_mod(y, y, n) + c) % n;
            y = (mul_mod(y, y, n) + c) % n;
            uint64_t diff = x > y ? x - y : y - x;
            d = u64_gcd(diff, n);
        }
        if (d != n) return d;
        ++c;
    }
}

/* recursive factor extraction */
static void factor_rec(uint64_t n, uint64_t fac[], size_t *cnt) {
    if (n == 1) return;
    if (is_prime(n)) {
        fac[(*cnt)++] = n;
        return;
    }
    uint64_t d = pollard_rho(n);
    factor_rec(d, fac, cnt);
    factor_rec(n / d, fac, cnt);
}

/* comparison for qsort */
static int cmp_uint64(const void *a, const void *b) {
    uint64_t va = *(const uint64_t *)a;
    uint64_t vb = *(const uint64_t *)b;
    if (va < vb) return -1;
    if (va > vb) return 1;
    return 0;
}

/* --------------------------------------------------------------- */
/* Public sequence function required by the test harness */
seq_t a(idx_t n) {
    if (n <= 1U) return (seq_t)0;
    uint64_t nn = (uint64_t)n;
    /* collect prime factors (with multiplicity) */
    uint64_t fac[64];
    size_t cnt = 0;
    factor_rec(nn, fac, &cnt);
    if (cnt == 0) return (seq_t)0;
    /* sort to group equal primes */
    qsort(fac, cnt, sizeof(uint64_t), cmp_uint64);
    /* count distinct primes whose exponent >= prime itself */
    seq_t ans = 0;
    size_t i = 0;
    while (i < cnt) {
        uint64_t p = fac[i];
        size_t exp = 0;
        while (i < cnt && fac[i] == p) { ++exp; ++i; }
        if (exp >= p) ++ans;
    }
    return ans;
}
