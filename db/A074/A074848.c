/*
 * A074848  Number of 4-infinitary divisors of n.
 * https://oeis.org/A074848
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Π_p Π_k (d_k+1), where the exponent of p in n written in
 *            base‑4 is e = Σ d_k·4^k. (The sequence is multiplicative.) The
 *            implementation factorises n, counts the prime powers, and
 *            multiplies the contributions of the base‑4 digits
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd64.h"

/* ---------- helper routines ---------- */

static inline uint64_t mul_mod(uint64_t a, uint64_t b, uint64_t mod) {
    return (uint64_t)((__int128)a * b % mod);
}

static inline uint64_t pow_mod(uint64_t a, uint64_t d, uint64_t mod) {
    uint64_t r = 1;
    while (d) {
        if (d & 1) r = mul_mod(r, a, mod);
        a = mul_mod(a, a, mod);
        d >>= 1;
    }
    return r;
}

/* deterministic Miller–Rabin for 64‑bit integers */
static inline int is_prime(uint64_t n) {
    if (n < 2) return 0;
    static uint64_t smallPrimes[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(smallPrimes)/sizeof(smallPrimes[0]); ++i) {
        if (n % smallPrimes[i] == 0) return n == smallPrimes[i];
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; s++; }
    uint64_t bases[] = {2,325,9375,28178,450775,9780504,1795265022};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = pow_mod(a, d, n);
        if (x == 1 || x == n-1) continue;
        int composite = 1;
        for (uint64_t r = 1; r < s; ++r) {
            x = mul_mod(x, x, n);
            if (x == n-1) { composite = 0; break; }
        }
        if (composite) return 0;
    }
    return 1;
}

/* Pollard Rho (Brent's variant) */
static uint64_t pollard(uint64_t n) {
    if ((n & 1) == 0) return 2;
    uint64_t c = 1, x = 2, y = 2, d = 1;
    while (d == 1) {
        x = (mul_mod(x, x, n) + c) % n;
        y = (mul_mod(y, y, n) + c) % n;
        y = (mul_mod(y, y, n) + c) % n;
        uint64_t diff = x > y ? x - y : y - x;
        d = gcd64(diff, n);
        if (d == n) { ++c; x = 2; y = 2; d = 1; }
    }
    return d;
}

/* recursively factor n; store unique primes and exponents in arrays */
static void factor_rec(uint64_t n, uint64_t *pkt, uint64_t *pexp, size_t *len) {
    if (n == 1) return;
    if (is_prime(n)) {
        for (size_t i = 0; i < *len; ++i) {
            if (pkt[i] == n) { ++pexp[i]; return; }
        }
        pkt[*len] = n; pexp[*len] = 1; (*len)++;
        return;
    }
    uint64_t d = pollard(n);
    factor_rec(d, pkt, pexp, len);
    factor_rec(n / d, pkt, pexp, len);
}

/* ---------- main sequence function ---------- */

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)1;

    uint64_t primes[16];          /* 2^64 has at most 16 distinct prime factors */
    uint64_t exps[16];
    size_t cnt = 0;

    factor_rec((uint64_t)n, primes, exps, &cnt);

    seq_t res = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e = exps[i];
        while (e) {
            uint64_t digit = e & 3ULL;          /* e % 4  */
            res *= (seq_t)(digit + 1ULL);       /* multiply by digit+1 */
            e >>= 2;                           /* e /= 4  */
        }
    }
    return res;
}
