/*
 * A063774  Numbers k such that the number of divisors of k^2 is a square.
 * https://oeis.org/A063774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A063774: Numbers k such that the number of divisors of k^2 is a square */
/* We generate terms by testing each integer k in order until we have n terms. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* --------------------------------------------------------------------------- */
/* Integer square root and perfect‑square test (no floating point)             */
/* --------------------------------------------------------------------------- */
static uint64_t floor_sqrt_u64(uint64_t x)
{
    uint64_t lo = 0, hi = 4294967295ULL; /* sqrt(UINT64_MAX) < 2^32 */
    while (lo < hi) {
        uint64_t mid = (lo + hi + 1) >> 1;
        if ((__uint128_t)mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

static bool is_square_uint64(uint64_t x)
{
    uint64_t r = floor_sqrt_u64(x);
    return r * r == x;
}

/* --------------------------------------------------------------------------- */
/* Linear sieve for smallest prime factor (SPF)                                 */
/* --------------------------------------------------------------------------- */
static uint32_t *spf  = NULL;      /* SPF array, indices 0…limit */
static size_t   spf_limit = 0;
static uint32_t *primes = NULL;    /* list of primes from the sieve */
static size_t   prime_cnt = 0;
static size_t   prime_cap = 0;

static void compute_sieve(size_t limit)
{
    if (limit == 0) return;
    /* free old data */
    if (spf)  free(spf);
    if (primes) free(primes);

    spf = (uint32_t*)calloc(limit + 1, sizeof(uint32_t));
    prime_cap = 16;
    primes = (uint32_t*)malloc(prime_cap * sizeof(uint32_t));
    prime_cnt = 0;

    for (size_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {                /* i is prime */
            spf[i] = (uint32_t)i;
            if (prime_cnt == prime_cap) {
                prime_cap <<= 1;
                primes = (uint32_t*)realloc(primes, prime_cap * sizeof(uint32_t));
            }
            primes[prime_cnt++] = (uint32_t)i;
        }
        for (size_t j = 0; j < prime_cnt; ++j) {
            uint32_t p = primes[j];
            uint64_t prod = (uint64_t)p * i;
            if (prod > limit) break;
            spf[prod] = p;
            if (i % p == 0) break;
        }
    }
    spf_limit = limit;
}

static void ensure_sieve_for_factor(uint64_t n)
{
    if (n <= spf_limit) return;
    size_t new_limit = spf_limit ? spf_limit : 128;
    while (new_limit < n) new_limit <<= 1;
    compute_sieve(new_limit);
}

/* --------------------------------------------------------------------------- */
/* Test whether τ(k²) is a perfect square                                      */
/* --------------------------------------------------------------------------- */
static bool good_k(uint64_t k)
{
    uint64_t tau = 1;                     /* τ(k²) = ∏ (2e_i + 1) */
    uint64_t m = k;
    while (m > 1) {
        uint32_t p = spf[m];
        uint32_t e = 0;
        while (m % p == 0) { m /= p; ++e; }
        tau *= (uint64_t)(2 * e + 1);
    }
    return is_square_uint64(tau);
}

/* --------------------------------------------------------------------------- */
/* Cache for generated sequence values                                         */
/* --------------------------------------------------------------------------- */
static seq_t *cache      = NULL;
static idx_t  cache_len  = 0;          /* number of values stored */
static idx_t  cache_cap  = 0;
static uint64_t last_checked_k = 0;

static void push_cache(seq_t val)
{
    if (cache_len == cache_cap) {
        cache_cap = cache_cap ? cache_cap << 1 : 16;
        cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
    }
    cache[cache_len++] = val;
}

static void ensure_n_terms(idx_t n)
{
    if (n == 0) return;
    while (cache_len < n) {
        ++last_checked_k;
        ensure_sieve_for_factor(last_checked_k);
        while (!good_k(last_checked_k)) {
            ++last_checked_k;
            ensure_sieve_for_factor(last_checked_k);
        }
        push_cache((seq_t)last_checked_k);
    }
}

/* --------------------------------------------------------------------------- */
/* Public sequence function                                                    */
/* --------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined by OEIS, but safe */
    if (cache == NULL) {
        cache = (seq_t*)malloc(16 * sizeof(seq_t));
        cache_len = 0;
        cache_cap = 16;
    }
    ensure_n_terms(n);
    return cache[n - 1];
}
