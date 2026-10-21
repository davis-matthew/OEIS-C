/*
 * A072589  n, phi(n) and sigma(n) end with the same digit.
 * https://oeis.org/A072589
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A072589: numbers n such that n, φ(n) and σ(n) end with the same digit. */
/* The function generates the sequence by testing successive integers
   and caching the successful ones.  It works for any seq_t that can
   be constructed from a uint64_t; thus it works for the arbitrary‑precision
   build as well as for the primitive integer builds. */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* ---- smallest prime factor (SPF) sieve --------------------------------- */
static uint64_t *spf = NULL;     /* spf[i] == 0 means i is prime during build */
static uint64_t spf_limit = 0;

/* Extend the SPF array to at least `n` (inclusive).  Uses a linear sieve. */
static void extend_spf(uint64_t n)
{
    if (n <= spf_limit) return;

    uint64_t new_limit = spf_limit ? spf_limit * 2 : 1024;
    while (new_limit < n) new_limit <<= 1;          /* double until enough */

    uint64_t *new_spf = (uint64_t*)calloc(new_limit + 1, sizeof(uint64_t));
    if (!new_spf) return;                           /* out‑of‑memory */

    uint64_t *primes = (uint64_t*)malloc((new_limit + 1) * sizeof(uint64_t));
    size_t prime_cnt = 0;

    for (uint64_t i = 2; i <= new_limit; ++i) {
        if (new_spf[i] == 0) {               /* i is prime */
            new_spf[i] = i;
            primes[prime_cnt++] = i;
        }
        for (size_t j = 0; j < prime_cnt; ++j) {
            uint64_t p = primes[j];
            uint64_t ip = i * p;
            if (ip > new_limit) break;
            new_spf[ip] = p;
            if (p == new_spf[i]) break;
        }
    }
    free(primes);

    free(spf);
    spf = new_spf;
    spf_limit = new_limit;
}

/* ---- Euler totient φ(n) using SPF ------------------------------------- */
static uint64_t phi_uint64(uint64_t n)
{
    if (n <= 1) return 1;                /* φ(1)=1 by convention */
    uint64_t result = n;
    uint64_t prev_p = 0;
    while (n > 1) {
        uint64_t p = spf[n];
        if (p == 0) p = n;               /* safety guard */
        if (p != prev_p) {
            result -= result / p;
            prev_p = p;
        }
        n /= p;
    }
    return result;
}

/* ---- Sum of divisors σ(n) using SPF ----------------------------------- */
static uint64_t sigma_uint64(uint64_t n)
{
    if (n <= 1) return 1;                /* σ(1)=1 */
    uint64_t result = 1;
    while (n > 1) {
        uint64_t p = spf[n];
        if (p == 0) p = n;
        unsigned exp = 0;
        while (n > 1 && spf[n] == p) {
            n /= p;
            ++exp;
        }
        uint64_t term = 1;
        for (unsigned i = 0; i <= exp; ++i) term *= p;
        term = (term - 1) / (p - 1);
        result *= term;
    }
    return result;
}

/* ---- Cache of sequence values ------------------------------------------ */
static uint64_t *cache = NULL;          /* stored numbers that satisfy condition */
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t next_candidate = 1;     /* next integer to test */

/* Append a found value to the cache. */
static void push_cache(uint64_t val)
{
    if (cache_len == cache_cap) {
        cache_cap = cache_cap ? cache_cap << 1 : 16;
        uint64_t *tmp = (uint64_t*)realloc(cache, cache_cap * sizeof(uint64_t));
        cache = tmp;
    }
    cache[cache_len++] = val;
}

/* ---- The required sequence function ------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                  /* undefined for offset 1 */

    while (cache_len < (size_t)n) {
        extend_spf(next_candidate);

        uint64_t phi = phi_uint64(next_candidate);
        uint64_t sigma = sigma_uint64(next_candidate);

        uint8_t last = (uint8_t)(next_candidate % 10);
        if ((uint8_t)(phi % 10) == last && (uint8_t)(sigma % 10) == last)
            push_cache(next_candidate);

        ++next_candidate;
    }
    return (seq_t)cache[n - 1];                    /* conversion from uint64_t */
}
