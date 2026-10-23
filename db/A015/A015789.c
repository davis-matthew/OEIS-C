/*
 * A015789  Numbers k such that phi(k) + 10 | sigma(k + 10).
 * https://oeis.org/A015789
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 47, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A015789: a(n) = the n-th integer k such that phi(k)+10 divides sigma(k+10) */
/* (phi = Euler's totient, sigma = sum of divisors).  Computed by incremental search
   with helper functions for totient and sigma that factor numbers by trial division
   using a small prime table generated once and reused.  */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --------- helper for integer square root --------- */
static uint64_t isqrt_u64(uint64_t n)
{
    uint64_t r = 0;
    uint64_t bit = 1ULL << 62;           /* highest power of four <= n */
    while (bit > n) bit >>= 2;
    while (bit != 0) {
        if (n >= r + bit) {
            n -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

/* --------- simple sieve for small primes --------- */
struct prime_sieve {
    uint32_t *primes;
    size_t   count;
    uint32_t limit;
} sieve = {NULL, 0, 0};

static void init_primes(uint32_t limit)
{
    if (sieve.limit >= limit) return;
    size_t size = limit + 1;
    bool *mark = (bool*)calloc(size, 1);
    for (uint32_t p = 2; p * p <= limit; ++p) {
        if (!mark[p]) {
            for (uint32_t j = p * p; j <= limit; j += p)
                mark[j] = true;
        }
    }
    size_t cnt = 0;
    for (uint32_t i = 2; i <= limit; ++i)
        if (!mark[i]) ++cnt;
    uint32_t *p = (uint32_t*)malloc(cnt * sizeof(uint32_t));
    size_t idx = 0;
    for (uint32_t i = 2; i <= limit; ++i)
        if (!mark[i]) p[idx++] = i;
    free(mark);
    free(sieve.primes);
    sieve.primes = p;
    sieve.count  = cnt;
    sieve.limit  = limit;
}

/* --------- factorization helpers --------- */
static seq_t xphi(uint64_t n)
{
    seq_t result = (seq_t)n;
    uint64_t rem = n;
    uint64_t sqrt_rem = isqrt_u64(rem);
    init_primes((uint32_t)sqrt_rem + 1);
    for (size_t i = 0; i < sieve.count && (uint64_t)sieve.primes[i] <= sqrt_rem; ++i) {
        uint64_t p = sieve.primes[i];
        if (rem % p == 0) {
            result -= result / (seq_t)p;
            while (rem % p == 0) rem /= p;
            sqrt_rem = isqrt_u64(rem);
        }
    }
    if (rem > 1) result -= result / (seq_t)rem;
    return result;
}

static seq_t xsigma(uint64_t n)
{
    seq_t result = 1;
    uint64_t rem = n;
    uint64_t sqrt_rem = isqrt_u64(rem);
    init_primes((uint32_t)sqrt_rem + 1);
    for (size_t i = 0; i < sieve.count && (uint64_t)sieve.primes[i] <= sqrt_rem; ++i) {
        uint64_t p = sieve.primes[i];
        if (rem % p == 0) {
            seq_t term = 1;
            seq_t pow  = 1;
            while (rem % p == 0) {
                rem /= p;
                pow *= p;
                term += pow;
            }
            result *= term;
            sqrt_rem = isqrt_u64(rem);
        }
    }
    if (rem > 1) result *= (1 + (seq_t)rem);
    return result;
}

/* --------- main sequence function --------- */
seq_t a(idx_t n)
{
    /* static cache for results */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0, cache_len = 0;
    static uint64_t next_k = 0;   /* next candidate k to test (>0) */

    if (n == 0) return 0;  /* sequence is 1‑based; undefined for 0 */

    /* enlarge cache if needed */
    if (cache == NULL) {
        cache_cap = 32;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    }

    /* generate terms until we have at least n of them */
    while (cache_len < n) {
        ++next_k;
        seq_t phi_k = xphi(next_k);
        seq_t denom = phi_k + (seq_t)10;
        seq_t sigma_val = xsigma(next_k + 10);
        if ( (sigma_val % denom) == 0 ) {
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_k;
        }
    }
    return cache[n - 1];
}
