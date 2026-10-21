/*
 * A055638  Numbers k for which sigma(k^2) is prime.
 * https://oeis.org/A055638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 47, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, isPrimeGeneric, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A055638 – Numbers k for which sigma(k^2) is prime.
   a(n) is the nth smallest integer k such that sigma(k^2) is prime,
   i.e. k = p^m where p is prime and (p^(2m+1)-1)/(p-1) is prime. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/big_pow.h"
#include "../utils/isPrimeGeneric.h"
#include "../utils/sigma_of.h"

/* --------------------------------------------------------------------- */
/* Prime sieve – enough for all prime factors of numbers encountered up
   to about 2^64.  The limit is 1 000 000, which exceeds the primes
   required for n ≤ 10 000 in this sequence.  */
#define PRIME_LIMIT 1000000U

static size_t prime_cnt = 0;
static uint32_t *primes = NULL;

static void init_primes(void)
{
    if (prime_cnt) return;         /* already built */

    bool *is_prime = (bool *)calloc(PRIME_LIMIT + 1, sizeof(bool));
    if (!is_prime) exit(EXIT_FAILURE);
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i) is_prime[i] = true;

    for (uint32_t p = 2; (uint64_t)p * p <= PRIME_LIMIT; ++p) {
        if (is_prime[p]) {
            for (uint32_t m = p * p; m <= PRIME_LIMIT; m += p)
                is_prime[m] = false;
        }
    }

    /* count primes */
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i)
        if (is_prime[i]) ++prime_cnt;

    primes = (uint32_t *)malloc(prime_cnt * sizeof(uint32_t));
    if (!primes) exit(EXIT_FAILURE);
    size_t idx = 0;
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i)
        if (is_prime[i]) primes[idx++] = i;

    free(is_prime);
}

/* --------------------------------------------------------------------- */
/* Cache for already generated terms */
static seq_t *seq_cache      = NULL;
static size_t seq_len        = 0;
static size_t seq_cap        = 0;
static uint64_t last_checked = 1;   /* last integer examined */

/* Ensure the cache has room for at least 'need' elements */
static void ensure_cache_cap(size_t need)
{
    if (need <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < need) new_cap <<= 1;
    seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!seq_cache) exit(EXIT_FAILURE);
    seq_cap = new_cap;
}

/* --------------------------------------------------------------------- */
/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* out-of-range, but safe */

    init_primes();

    /* Extend cache until the requested index is available */
    while (seq_len < (size_t)n) {
        ++last_checked;
        uint64_t k = last_checked;
        if (k <= 1) continue;

        /* Find smallest prime factor and exponent */
        uint64_t temp = k;
        uint64_t p = 0;
        int m = 0;
        bool is_prime_power = true;

        for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * primes[i] <= temp; ++i) {
            uint32_t pr = primes[i];
            if (temp % pr == 0) {
                p = pr;
                while (temp % pr == 0) {
                    temp /= pr;
                    ++m;
                }
                break;
            }
        }

        if (temp == k) {             /* no small factor found */
            /* Treat as prime; exponent m=1 */
            p = k;
            m = 1;
        } else {
            if (temp != 1) is_prime_power = false;   /* extra factor remains */
        }

        if (!is_prime_power) continue;           /* not a pure prime power */

        /* Compute sigma(p^(2m)) = (p^(2m+1)-1)/(p-1)  */
        seq_t p_seq = (seq_t)p;
        seq_t exp   = (seq_t)(2 * m + 1);
        seq_t numerator   = big_pow(p_seq, exp) - (seq_t)1;
        seq_t denominator = p_seq - (seq_t)1;
        seq_t sigma = numerator / denominator;

        if (isPrimeGeneric(sigma)) {
            ensure_cache_cap(seq_len + 1);
            seq_cache[seq_len++] = (seq_t)k;
        }
    }

    return seq_cache[n - 1];
}
