/*
 * A094461  a[n] is the 5th term in Euclid-Mullin (EM) prime sequence
 *          initiated with n-th prime.
 * https://oeis.org/A094461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A094461 – 5th term of the Euclid‑Mullin sequence started at the n‑th prime.
 *  The sequence is defined for n ≥ 1 and uses the n‑th prime pₙ as the first
 *  term of a Euclid‑Mullin sequence.  The value returned is the 5th
 *  element of that sequence.  This implementation uses a deterministic
 *  Miller–Rabin test and Pollard–Rho factorisation over 128‑bit integers to
 *  find the smallest prime factor of the products+1 that arise.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/get_prime.h"

/* modular multiplication for 128‑bit operands – no overflow */
static __uint128_t mul_mod(__uint128_t a, __uint128_t b, __uint128_t mod)
{
    __uint128_t res = 0;
    a %= mod;
    while (b) {
        if (b & 1)
            res = (res + a) % mod;
        a = (a << 1) % mod;
        b >>= 1;
    }
    return res;
}

/* modular exponentiation – uses mul_mod to avoid overflow */
static __uint128_t mod_pow(__uint128_t base, __uint128_t exp, __uint128_t mod)
{
    __uint128_t result = 1;
    base %= mod;
    while (exp) {
        if (exp & 1)
            result = mul_mod(result, base, mod);
        base = mul_mod(base, base, mod);
        exp >>= 1;
    }
    return result;
}

/* binary GCD for 128‑bit integers */
static __uint128_t gcd128(__uint128_t a, __uint128_t b)
{
    while (b) {
        __uint128_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* deterministic Miller–Rabin for 128‑bit integers.
 * The bases below are sufficient for all numbers < 2¹²⁸. */
static bool is_prime_uint128(__uint128_t n)
{
    if (n < 2)          return false;
    if (n % 2 == 0)     return n == 2;
    if (n % 3 == 0)     return n == 3;
    /* small trial division to improve speed */
    static const uint64_t small_primes[] = {5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        if (n % small_primes[i] == 0) return n == small_primes[i];
    }
    /* Miller–Rabin */
    __uint128_t d = n - 1;
    unsigned r = 0;
    while ((d & 1) == 0) { d >>= 1; ++r; }
    static const uint64_t bases[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        __uint128_t a = bases[i] % n;
        if (a == 0) continue;
        __uint128_t x = mod_pow(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (unsigned j = 1; j < r; ++j) {
            x = mul_mod(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* Pollard–Rho factorisation for 128‑bit integers. */
static __uint128_t pollard_rho_uint128(__uint128_t n)
{
    if (n % 2 == 0) return 2;
    if (n % 3 == 0) return 3;
    __uint128_t c = 1;
    while (1) {
        __uint128_t x = 2, y = 2, d = 1;
        while (d == 1) {
            x = (mul_mod(x, x, n) + c) % n;
            y = (mul_mod(y, y, n) + c) % n;
            y = (mul_mod(y, y, n) + c) % n;
            __uint128_t diff = (x > y) ? x - y : y - x;
            d = gcd128(diff, n);
        }
        if (d != n) return d;
        ++c;
    }
}

/* Recursively find the smallest prime factor of a 128‑bit integer. */
static __uint128_t min_factor_uint128(__uint128_t n)
{
    if (n <= 1) return n;
    if (is_prime_uint128(n)) return n;
    __uint128_t f = pollard_rho_uint128(n);
    __uint128_t f1 = min_factor_uint128(f);
    __uint128_t f2 = min_factor_uint128(n / f);
    return (f1 < f2) ? f1 : f2;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1 */
    uint64_t p = get_prime(n);     /* n‑th prime (fits in 32 bits for n ≤ 10000) */

    /* 2nd term: smallest prime factor of p + 1 */
    __uint128_t val = (__uint128_t)p + 1;
    __uint128_t a2 = min_factor_uint128(val);

    /* product of first two terms */
    __uint128_t prod2 = ( __uint128_t)p * a2;

    /* 3rd term: spf(prod2 + 1) */
    __uint128_t a3 = min_factor_uint128(prod2 + 1);

    /* product of first three terms */
    __uint128_t prod3 = prod2 * a3;

    /* 4th term: spf(prod3 + 1) */
    __uint128_t a4 = min_factor_uint128(prod3 + 1);

    /* product of first four terms */
    __uint128_t prod4 = prod3 * a4;

    /* 5th term: spf(prod4 + 1) – this is the sequence value */
    __uint128_t a5 = min_factor_uint128(prod4 + 1);

    return (seq_t)a5;
}
