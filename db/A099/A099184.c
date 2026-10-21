/*
 * A099184  Heavy primes: primes p such that p-1 has more than 2 divisors with
 *          multiplicity.
 * https://oeis.org/A099184
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: a(n) = the n‑th prime p (n ≥ 2) such that Ω(p−1) > 2. */
/* The function builds a linear sieve, gathers heavy primes into a cache, */
/* and returns the n‑th heavy prime.  It is order‑independent and works   */
/* for any integer types seq_t and idx_t supplied by the build.          */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Limit of the sieve – enough for ~10 000 heavy primes. */
static const size_t SIEVE_LIMIT = 500000;

static bool sieve_ready = false;

/* Sieve data. */
static uint32_t *spf = NULL;          /* smallest prime factor   */
static uint32_t *primes = NULL;       /* list of primes          */
static size_t prime_cnt = 0;
static size_t prime_idx = 0;          /* next prime to test      */

/* Cache of heavy primes. */
static seq_t *heavy_cache = NULL;
static size_t heavy_len = 0;
static size_t heavy_cap = 0;

/* Build a linear sieve up to SIEVE_LIMIT. */
static void build_sieve(void)
{
    spf     = (uint32_t*)malloc((SIEVE_LIMIT + 1) * sizeof(uint32_t));
    if (!spf) exit(EXIT_FAILURE);
    primes  = (uint32_t*)malloc((SIEVE_LIMIT / 10 + 100) * sizeof(uint32_t));
    if (!primes) exit(EXIT_FAILURE);

    spf[0] = spf[1] = 0;
    size_t pc = 0;
    for (size_t i = 2; i <= SIEVE_LIMIT; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            primes[pc++] = (uint32_t)i;
        }
        for (size_t j = 0; j < pc &&
             primes[j] <= spf[i] &&
             (uint64_t)primes[j] * i <= SIEVE_LIMIT; ++j) {
            spf[primes[j] * i] = primes[j];
        }
    }
    prime_cnt = pc;
    sieve_ready = true;
}

/* Compute Ω(n) – total number of prime factors with multiplicity. */
static size_t big_omega(uint32_t n)
{
    size_t w = 0;
    while (n > 1) {
        uint32_t p = spf[n];
        w++;
        n /= p;
    }
    return w;
}

/* Append a new heavy prime to the cache, growing the storage if needed. */
static void cache_append(seq_t val)
{
    if (heavy_len == heavy_cap) {
        size_t new_cap = (heavy_cap == 0) ? 16 : heavy_cap * 2;
        seq_t *tmp = (seq_t*)realloc(heavy_cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        heavy_cache = tmp;
        heavy_cap = new_cap;
    }
    heavy_cache[heavy_len++] = val;
}

/* Sequence value a(n).  n == 2 corresponds to the first heavy prime. */
seq_t a(idx_t n)
{
    if (n < 2) return 0;  /* Undefined for offset < 2 */

    if (!sieve_ready) build_sieve();

    /* Ensure we have at least n‑1 heavy primes cached. */
    while (heavy_len < (size_t)(n - 1)) {
        if (prime_idx >= prime_cnt) {
            /* Should not happen with the chosen sieve limit. */
            exit(EXIT_FAILURE);
        }
        uint32_t p = primes[prime_idx++];
        if (big_omega(p - 1) > 2) {
            cache_append((seq_t)p);
        }
    }
    return heavy_cache[(size_t)(n - 2)];
}
