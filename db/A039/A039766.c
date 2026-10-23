/*
 * A039766  Numbers k such that gcd(phi(k), k-1) = number of divisors of k.
 * https://oeis.org/A039766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 4613, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A039766: numbers k such that gcd(phi(k), k-1) = d(k) */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/gcd.h"

/* -------------------------------------------------------------
   Prime sieve (up to a fixed limit) used for factorizations
   ------------------------------------------------------------- */
static uint32_t *primes = NULL;
static size_t primes_cnt = 0;
static size_t primes_cap = 0;
static bool primes_inited = false;
static const uint32_t PRIME_LIMIT = 1000000;   /* sufficient for the ranges needed */

static void init_primes(void)
{
    if (primes_inited) return;

    uint32_t n = PRIME_LIMIT;
    bool *is_comp = (bool *)calloc(n + 1, 1);
    if (!is_comp) abort();

    primes_cap = n / 10;
    primes = (uint32_t *)malloc(primes_cap * sizeof(uint32_t));
    if (!primes) {
        free(is_comp);
        abort();
    }

    for (uint32_t i = 2; i <= n; ++i) {
        if (!is_comp[i]) {
            if (primes_cnt == primes_cap) {
                primes_cap *= 2;
                uint32_t *tmp = (uint32_t *)realloc(primes, primes_cap * sizeof(uint32_t));
                if (!tmp) { free(is_comp); free(primes); abort(); }
                primes = tmp;
            }
            primes[primes_cnt++] = i;
            if ((uint64_t)i * i <= n) {
                for (uint32_t j = i * i; j <= n; j += i) is_comp[j] = 1;
            }
        }
    }
    free(is_comp);
    primes_inited = true;
}

/* -------------------------------------------------------------
   Factorization, phi, and divisor count
   ------------------------------------------------------------- */
static void factor_phi_divcount(uint64_t n, seq_t *phi_out, size_t *d_out)
{
    uint64_t x = n;
    seq_t phi = (seq_t)n;
    size_t d = 1;

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint32_t p = primes[i];
        if ((uint64_t)p * (uint64_t)p > x) break;
        if (x % p == 0) {
            size_t e = 0;
            do {
                x /= p;
                ++e;
            } while (x % p == 0);
            phi = phi / (seq_t)p * ((seq_t)p - 1);
            d *= (e + 1);
        }
    }
    if (x > 1) {
        seq_t p = (seq_t)x;
        phi = phi / p * (p - 1);
        d *= 2;
    }
    *phi_out = phi;
    *d_out = d;
}

/* -------------------------------------------------------------
   Cache of sequence values
   ------------------------------------------------------------- */
static seq_t *seq_cache = NULL;
static size_t cache_sz = 0;
static size_t cache_cap = 0;
static uint64_t next_candidate = 1;   /* next k to test */

static void cache_append(uint64_t val)
{
    if (cache_sz == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        seq_cache = tmp;
        cache_cap = new_cap;
    }
    seq_cache[cache_sz++] = (seq_t)val;
}

/* -------------------------------------------------------------
   The required function
   ------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; below 1 is irrelevant */

    if (!primes_inited) init_primes();

    while (cache_sz < (size_t)n) {
        uint64_t k = next_candidate++;
        seq_t phi;
        size_t d;
        factor_phi_divcount(k, &phi, &d);
        seq_t g = gcd(phi, (seq_t)(k - 1));
        if (g == (seq_t)d) {
            cache_append(k);
        }
    }
    return seq_cache[(size_t)n - 1];
}
