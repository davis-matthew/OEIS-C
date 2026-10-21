/*
 * A217048  Semiprimes using only the curved digits 0, 3, 6, 8 and 9.
 * https://oeis.org/A217048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 311, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A217048: Semiprimes using only the curved digits 0,3,6,8,9.
   Enumerate all numbers whose decimal digits belong to {0,3,6,8,9}
   in increasing order and keep those that are semiprime (exactly two prime
   factors, counting multiplicity). */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ */
/* Global static data                                                 */
/* ------------------------------------------------------------------ */
static seq_t *cache      = NULL;    /* cached terms of the sequence          */
static size_t cache_len  = 0;        /* number of cached elements             */
static size_t cache_cap  = 0;        /* capacity of the cache                 */

static uint32_t *prime_table = NULL;   /* primes up to LIMIT            */
static size_t   prime_cnt   = 0;       /* number of primes in table      */
static bool     primes_built = false;

static const uint8_t allowed_digits[5] = {0,3,6,8,9};
static int last_generated_len = 0;    /* last length processed            */

/* ------------------------------------------------------------------ */
/* Simple primality test for 64‑bit numbers (used internally)         */
/* ------------------------------------------------------------------ */
static bool is_prime_small(uint64_t n) {
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (uint64_t d = 3; d * d <= n; d += 2)
        if (n % d == 0) return false;
    return true;
}

/* ------------------------------------------------------------------ */
/* Build a prime sieve up to LIMIT.                                  */
/* ------------------------------------------------------------------ */
static void build_prime_sieve(void) {
    const uint32_t LIMIT = 200000;
    bool *is_prime = (bool *)calloc((size_t)LIMIT + 1, sizeof(bool));
    if (!is_prime) exit(1);
    for (uint32_t i = 2; i <= LIMIT; ++i) is_prime[i] = true;
    for (uint32_t p = 2; (uint64_t)p * p <= LIMIT; ++p) {
        if (is_prime[p]) {
            for (uint32_t k = p * p; k <= LIMIT; k += p)
                is_prime[k] = false;
        }
    }
    for (uint32_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i]) ++prime_cnt;
    prime_table = (uint32_t *)malloc(prime_cnt * sizeof(uint32_t));
    if (!prime_table) exit(1);
    size_t idx = 0;
    for (uint32_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i]) prime_table[idx++] = i;
    free(is_prime);
    primes_built = true;
}

/* ------------------------------------------------------------------ */
/* Cache handling                                                     */
/* ------------------------------------------------------------------ */
static void cache_push(seq_t val) {
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1024;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* ------------------------------------------------------------------ */
/* Test whether n is a semiprime.                                     */
/* ------------------------------------------------------------------ */
static bool is_semiprime(uint64_t n) {
    if (n < 6) return false;          /* 2*3 = 6 is the smallest */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = prime_table[i];
        uint64_t p2 = (uint64_t)p * (uint64_t)p;
        if (p2 > n) break;
        if (n % p == 0) {
            uint64_t q = n / p;
            if (q == p) return true;   /* square of a prime */
            /* check that q is prime                                 */
            for (size_t j = 0; j < prime_cnt &&
                     (uint64_t)prime_table[j] * prime_table[j] <= q; ++j) {
                if (q % prime_table[j] == 0) return false;
            }
            return true;
        }
    }
    return false; /* prime or composite with more than two prime factors */
}

/* ------------------------------------------------------------------ */
/* Recursive generator of numbers of a given digit length             */
/* ------------------------------------------------------------------ */
static void generate_rec(uint64_t current, int depth, int target) {
    if (depth == target) {
        if (is_semiprime(current))
            cache_push((seq_t)current);
        return;
    }
    for (int d = 0; d < 5; ++d) {
        uint8_t digit = allowed_digits[d];
        if (depth == 0 && digit == 0) continue;   /* no leading zero */
        generate_rec(current * 10 + digit, depth + 1, target);
    }
}

static void generate_len(int len) { generate_rec(0, 0, len); }

/* ------------------------------------------------------------------ */
/* Ensure cache contains at least n terms.                             */
/* ------------------------------------------------------------------ */
static void ensure_cache(idx_t n) {
    if (!primes_built) build_prime_sieve();
    while (cache_len < (size_t)n) {
        ++last_generated_len;
        generate_len(last_generated_len);
    }
}

/* ------------------------------------------------------------------ */
/* Requested function                                                  */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* undefined for n=0, fallback   */
    ensure_cache(n);
    return cache[(size_t)n - 1];
}
