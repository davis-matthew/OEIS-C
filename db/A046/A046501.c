/*
 * A046501  Primes with multiplicative persistence value 1.
 * https://oeis.org/A046501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 1751, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_product
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by searching primes: a(n) is the nth prime (p > 9) whose
   product of digits is ≤ 9 (including 0 or 1).  The primes 2,3,5,7 are
   omitted because numbers < 10 have persistence 0.  The function
   works for any seq_t, with a static cache of found elements. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/digit_product.h"

/* Catalysed array of qualifying primes. */
static seq_t *qualifiers = NULL;
static size_t qual_cap   = 0;
static size_t qual_len   = 0;

/* Next integer to test for primality.  Start above 9.  */
static uint64_t next_candidate = 10;

/* Append a new qualifying prime to the cache. */
static void add_qualifier(seq_t p)
{
    if (qual_len == qual_cap) {
        size_t new_cap = qual_cap ? qual_cap * 2 : 32;
        seq_t *tmp = (seq_t *)realloc(qualifiers, new_cap * sizeof(seq_t));
        qualifiers = tmp;
        qual_cap   = new_cap;
    }
    qualifiers[qual_len++] = p;
}

/* 64‑bit modular multiplication using __int128 (no overflow). */
static inline uint64_t mul_mod(uint64_t a, uint64_t b, uint64_t m)
{
    return (unsigned __int128)a * b % m;
}

/* 64‑bit modular exponentiation. */
static uint64_t mod_pow(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    base %= mod;
    while (exp) {
        if (exp & 1) result = mul_mod(result, base, mod);
        base = mul_mod(base, base, mod);
        exp >>= 1;
    }
    return result;
}

/* Deterministic Miller–Rabin for 64‑bit integers. */
static bool is_prime_64(uint64_t n)
{
    if (n < 2)            return false;
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0)     return false;

    /* Write n-1 = d * 2^s with d odd. */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    /* Bases sufficient for < 2^64. */
    const uint64_t bases[] = {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = mod_pow(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = mul_mod(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* Grow the cache until it contains at least target_n elements. */
static void ensure_qualifiers(idx_t target_n)
{
    while (qual_len < (size_t)target_n) {
        if (is_prime_64(next_candidate)) {
            seq_t p = (seq_t)next_candidate;
            seq_t prod = digit_product(p);
            if (prod <= (seq_t)9) add_qualifier(p);
        }
        ++next_candidate;
    }
}

/* External function required by the framework. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;         /* undefined for offset 1 */
    ensure_qualifiers(n);
    return qualifiers[(size_t)(n - 1)];
}
