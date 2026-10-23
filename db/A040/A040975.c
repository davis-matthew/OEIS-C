/*
 * A040975  Primes p such that x^5 = 15 has no solution mod p.
 * https://oeis.org/A040975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A040975: primes p such that the congruence x^5 = 15 (mod p) has no solution. */

/* The test is: 15 is a 5th‑power residue modulo p iff
 * 15^((p‑1)/gcd(p‑1,5)) ≡ 1 (mod p).
 * Hence we record the primes p>5 with 15^((p‑1)/gcd(p‑1,5)) ≠ 1. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "../utils/is_prime_small.h"

static inline uint64_t modmul_uint64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (unsigned __int128)a * b % mod;
}

static inline uint64_t modpow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    uint64_t b = base % mod;
    while (exp) {
        if (exp & 1) result = modmul_uint64(result, b, mod);
        b = modmul_uint64(b, b, mod);
        exp >>= 1;
    }
    return result;
}

/* ensure that cache has capacity at least cap (number of seq_t elements) */
static void ensure_cache_capacity(idx_t cap)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    if (cache_cap < cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < cap) new_cap <<= 1;
        seq_t *newp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!newp) abort();            /* fatal: out of memory */
        cache = newp;
        cache_cap = new_cap;
    }
}

/* The sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n<1, harmless */

    /* static caches (independent per translation unit) */
    static seq_t *prime_cache = NULL;  /* primes satisfying the condition */
    static size_t prime_cache_len = 0;
    static size_t prime_cache_cap = 0;
    static uint64_t candidate = 7;     /* start after 5 */

    /* growth helper for prime_cache */
    if (prime_cache_len == 0 && prime_cache_cap == 0) {
        prime_cache_cap = 16;
        prime_cache = (seq_t *)malloc(prime_cache_cap * sizeof(seq_t));
        if (!prime_cache) abort();   /* fatal: out of memory */
    }

    /* fill cache until we have n elements */
    while (prime_cache_len < n) {
        /* find next prime >5 */
        while (true) {
            /* trial divide by small primes via utility */
            if (candidate <= 5) { candidate = 7; continue; }
            if ((candidate & 1) == 0) { candidate += 1; continue; }

            if (is_prime_small((seq_t)candidate)) break;

            candidate += 2;
        }

        uint64_t p = candidate;

        /* test 15 is NOT a fifth power residue modulo p */
        uint64_t exp = p - 1;
        if ((p - 1) % 5ULL == 0) exp = (p - 1) / 5ULL;

        uint64_t val = modpow_uint64(15ULL, exp, p);

        if (val != 1ULL) {
            /* store prime in cache */
            if (prime_cache_len == prime_cache_cap) {
                size_t new_cap = prime_cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(prime_cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                prime_cache = tmp;
                prime_cache_cap = new_cap;
            }
            prime_cache[prime_cache_len++] = (seq_t)p;
        }

        /* move to next odd candidate */
        candidate += 2;
    }

    return prime_cache[n - 1];
}
