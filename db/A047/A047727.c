/*
 * A047727  Average divisor is an integer (A003601) and the number is
 *          refactorable (A033950).
 * https://oeis.org/A047727
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 632, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Condition: numbers x for which both sigma(x)/tau(x) and x/tau(x) are integers. */
/* The function returns the nth such number, with a(1)=1. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

static bool primes_inited = false;
static size_t primes_cnt   = 0;
static size_t primes_cap   = 0;
static uint32_t *primes    = NULL;   /* list of primes */

static void init_primes(void)
{
    if (primes_inited) return;
    /* Sieve up to a generous limit; primes up to 1,000,000 are more than enough */
    const uint32_t LIMIT = 1000000U;
    bool *is_prime = (bool *)malloc((LIMIT + 1) * sizeof(bool));
    if (!is_prime) exit(1);
    for (uint32_t i = 0; i <= LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (uint32_t i = 2; i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (uint32_t j = i * i; j <= LIMIT; j += i) is_prime[j] = false;
        }
    }
    /* Count primes */
    size_t cnt = 0;
    for (uint32_t i = 2; i <= LIMIT; ++i) if (is_prime[i]) ++cnt;
    primes_cnt = cnt;
    primes_cap = cnt;
    primes = (uint32_t *)malloc(primes_cap * sizeof(uint32_t));
    if (!primes) exit(1);
    size_t pos = 0;
    for (uint32_t i = 2; i <= LIMIT; ++i) if (is_prime[i]) primes[pos++] = i;
    free(is_prime);
    primes_inited = true;
}

/* Compute sigma(n) and tau(n) for n <= UINT64_MAX. */
static void factor_sigma_tau(uint64_t n, seq_t *sigma, seq_t *tau)
{
    uint64_t temp = n;
    *sigma = (seq_t)1;
    *tau   = (seq_t)1;

    for (size_t i = 0; i < primes_cnt && (uint64_t)primes[i] * primes[i] <= temp; ++i) {
        uint32_t p = primes[i];
        if (temp % p == 0) {
            int e = 0;
            /* compute p^e */
            seq_t p_pow = (seq_t)1;
            while (temp % p == 0) {
                temp /= p;
                ++e;
                p_pow *= (seq_t)p;
            }
            /* update tau */
            *tau *= (seq_t)(e + 1);
            /* compute sigma factor: (p^{e+1}-1)/(p-1) */
            seq_t numerator   = p_pow * (seq_t)p - (seq_t)1;  /* p^{e+1} - 1 */
            seq_t denominator = (seq_t)p - (seq_t)1;          /* p - 1 */
            seq_t factor      = numerator / denominator;     /* integer division */
            *sigma *= factor;
        }
    }
    if (temp > 1) {        /* remaining prime factor */
        seq_t temp_seq = (seq_t)temp;
        *tau   *= (seq_t)2;
        *sigma *= (temp_seq + (seq_t)1);
    }
}

/* Cache for discovered terms */
static seq_t *cache   = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static uint64_t last_checked = 0;

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* guard against invalid input */

    /* Allocate cache on first call. */
    if (!cache) {
        cache_cap = 64;
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(1);
        cache_len = 0;
        last_checked = 0;          /* start searching from 1 */
    }

    /* Ensure primes are ready. */
    init_primes();

    /* Extend the cache until we have at least n terms. */
    while (cache_len < (size_t)n) {
        ++last_checked;
        seq_t tau, sigma;
        factor_sigma_tau(last_checked, &sigma, &tau);

        if (((seq_t)last_checked % tau == 0) && (sigma % tau == 0)) {
            /* Store the term. */
            if (cache_len >= cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!cache) exit(1);
            }
            cache[cache_len++] = (seq_t)last_checked;
        }
    }

    return cache[n - 1];
}
