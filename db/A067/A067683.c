/*
 * A067683  Numbers k such that phi(k)*sigma(k) + 1 is prime.
 * https://oeis.org/A067683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Numbers k such that phi(k)*sigma(k)+1 is prime
 * Range:     u8 n <= 6, u16 n <= 252, u32 n <= 58, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function returns the n-th term of that sequence. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/is_prime_big.h"

static seq_t *seq_cache = NULL;          /* cached sequence values */
static size_t seq_len   = 0;
static size_t seq_cap   = 0;
static uint64_t next_k  = 1;             /* next k to test */

static uint32_t *primes = NULL;          /* list of small primes for factorisation */
static size_t   primes_cnt = 0;
static uint32_t  primes_limit = 0;

/* ensure we have a prime table up to limit */
static void build_prime_table(uint32_t limit)
{
    if (limit <= primes_limit)
        return;
    /* simple sieve up to limit */
    size_t sz = (size_t)limit + 1;
    bool *sieve = (bool *)malloc(sz);
    if (!sieve) abort();
    memset(sieve, 1, sz);
    sieve[0] = sieve[1] = 0;
    for (uint32_t i = 2; (uint64_t)i * i <= limit; ++i) {
        if (sieve[i]) {
            for (uint64_t j = (uint64_t)i * i; j <= limit; j += i)
                sieve[(size_t)j] = 0;
        }
    }
    /* count primes */
    size_t cnt = 0;
    for (uint32_t i = 2; i <= limit; ++i)
        if (sieve[i]) ++cnt;
    /* allocate */
    uint32_t *new_pr = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    if (!new_pr) abort();
    size_t idx = 0;
    for (uint32_t i = 2; i <= limit; ++i)
        if (sieve[i]) new_pr[idx++] = i;
    free(sieve);
    free(primes);
    primes = new_pr;
    primes_cnt = cnt;
    primes_limit = limit;
}

/* ensure the cache has capacity for n items */
static void cache_ensure_cap(size_t needed)
{
    if (needed <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!seq_cache) abort();
    seq_cap = new_cap;
}

/* compute phi(k) and sigma(k) for uint64_t k */
static void compute_phi_sigma(uint64_t k, seq_t *phi_out, seq_t *sigma_out)
{
    /* ensure we have primes up to sqrt(k) */
    uint32_t sqrt_k = (uint32_t)((uint64_t)((double)k * 0.5) + 1);
    while ((uint64_t)sqrt_k * sqrt_k < k) ++sqrt_k;
    build_prime_table(sqrt_k);

    seq_t phi = 1;
    seq_t sigma = 1;
    uint64_t n = k;

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint32_t p = primes[i];
        if ((uint64_t)p * (uint64_t)p > n)
            break;
        if (n % p == 0) {
            size_t e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            /* compute p^e */
            seq_t pow = 1;
            for (size_t j = 0; j < e; ++j)
                pow *= (seq_t)p;               /* pow = p^e */
            /* phi factor: (p-1)*p^(e-1) */
            seq_t phi_factor = (seq_t)(p - 1) * (pow / (seq_t)p);
            phi *= phi_factor;
            /* sigma factor: (p^(e+1)-1)/(p-1) */
            seq_t pow_next = pow * (seq_t)p;   /* p^(e+1) */
            seq_t sigma_factor = (pow_next - 1) / (seq_t)(p - 1);
            sigma *= sigma_factor;
        }
    }
    if (n > 1) {          /* remaining prime factor */
        seq_t p = (seq_t)n;
        /* e = 1 */
        phi *= (p - 1);
        sigma *= (p + 1);
    }
    *phi_out = phi;
    *sigma_out = sigma;
}

seq_t a(idx_t n)
{
    /* maintain cache of results */
    while (seq_len < (size_t)n) {
        seq_t phi, sigma;
        compute_phi_sigma(next_k, &phi, &sigma);
        seq_t d = phi * sigma + (seq_t)1;
        if (is_prime_big(d)) {
            cache_ensure_cap(seq_len + 1);
            seq_cache[seq_len++] = next_k;
        }
        ++next_k;
    }
    return seq_cache[(size_t)n - 1];
}
