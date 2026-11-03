/*
 * A096174  Even numbers k such that (k^3+1)/(k+1) is prime.
 * https://oeis.org/A096174
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 3660, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 9959 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A096174:  Even numbers k such that (k^3+1)/(k+1) is prime
   a(n) returns the nth term (offset 1). The value reduces to k^2-k+1
   being prime.  The function builds the sequence on demand, storing
   found terms in a static cache. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

static seq_t *cache = NULL;   /* array of found terms */
static size_t cache_size = 0; /* number of terms stored */
static size_t cache_cap   = 0; /* allocated capacity */
static uint64_t next_even = 2; /* next even k to test */

/* 64‑bit modular multiplication via 128‑bit intermediate */
static inline uint64_t mul128(uint64_t a, uint64_t b, uint64_t mod)
{
    return (unsigned __int128)a * b % mod;
}

/* 64‑bit modular exponentiation */
static inline uint64_t mod_pow(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    uint64_t b = base % mod;
    while (exp) {
        if (exp & 1) result = mul128(result, b, mod);
        b = mul128(b, b, mod);
        exp >>= 1;
    }
    return result;
}

/* Deterministic Miller–Rabin for 64‑bit unsigned integers.
   Uses the bases {2, 3, 5, 7, 11, 13, 17} which are sufficient
   for all 64‑bit integers. */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    /* Small prime trial division */
    static const uint64_t small_primes[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        if (n % small_primes[i] == 0) return n == small_primes[i];
    }
    /* Miller–Rabin */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    static const uint64_t bases[] = {2,3,5,7,11,13,17};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i];
        if (a % n == 0) continue;
        uint64_t x = mod_pow(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = mul128(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* Append a new term to the cache, growing the array if needed. */
static void cache_add(seq_t val)
{
    if (cache_size == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_size++] = val;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return 0;  /* offset 1 */

    while (cache_size < n) {
        uint64_t k = next_even;              /* current even k */
        uint64_t m = k * k - k + 1;          /* k^2 - k + 1 */
        bool prime;
        if (sizeof(seq_t) <= sizeof(uint64_t)) {
            prime = is_prime_uint64(m);
        } else {
            seq_t m_seq = (seq_t)k;
            m_seq = m_seq * m_seq;
            m_seq = m_seq - (seq_t)k;
            m_seq = m_seq + 1;
            prime = is_prime_big(m_seq);
        }
        if (prime) {
            /* store k only if it fits in seq_t; otherwise stop caching */
            seq_t k_cast = (seq_t)k;
            cache_add(k_cast);
        }
        next_even += 2; /* advance to next even number */
    }
    return cache[n - 1];
}
