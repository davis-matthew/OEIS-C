/*
 * A046642  Numbers k such that k and number of divisors d(k) are relatively
 *          prime.
 * https://oeis.org/A046642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = numbers k such that gcd(k, τ(k)) == 1 * (Anti‑tau
 *            numbers) * Sequence a(1)=1, a(2)=3, a(3)=4, a(4)=5, a(5)=7,
 *            a(6)=11, …
 * Range:     u8 n <= 121, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* --------------------------- required headers --------------------------- */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"

/* --------------------------- static data ------------------------------ */
static seq_t *cache      = NULL;   /* cache of found terms          */
static size_t cache_cap  = 0;      /* allocated capacity in cache   */
static size_t cache_len  = 0;      /* number of terms stored       */

static uint32_t *primes   = NULL;  /* prime list                    */
static size_t   prime_cnt = 0;     /* number of primes currently    */
static uint64_t prime_lim = 0;     /* largest number up to which
                                     primes are known              */

/* --------------------------- helper: grow prime list ------------------- */
static void grow_primes_to(uint64_t limit)
{
    if (limit <= prime_lim) return;               /* nothing to do */

    /* Sieve of Eratosthenes up to 'limit' */
    size_t sz = limit + 1;
    bool *sieve = (bool*)malloc(sz);
    if (!sieve) abort();                 /* allocation failure */
    for (size_t i = 0; i < sz; ++i) sieve[i] = true;
    sieve[0] = sieve[1] = false;

    for (uint64_t i = 2; i * i <= limit; ++i) {
        if (sieve[i]) {
            for (uint64_t j = i * i; j <= limit; j += i)
                sieve[j] = false;
        }
    }

    /* Count primes and gather them */
    size_t new_cnt = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (sieve[i]) ++new_cnt;

    uint32_t *new_pr = (uint32_t*)malloc(new_cnt * sizeof(uint32_t));
    if (!new_pr) abort();

    size_t idx = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (sieve[i]) new_pr[idx++] = (uint32_t)i;

    /* Replace old prime list */
    free(primes);
    primes = new_pr;
    prime_cnt = prime_lim = new_cnt;

    free(sieve);
}

/* --------------------------- divisor count ---------------------------- */
static seq_t tau_of_uint64(uint64_t n)
{
    seq_t tau = 1;
    uint64_t tmp = n;

    /* Ensure we have enough primes to factorize 'tmp' */
    grow_primes_to(tmp);

    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * primes[i] <= tmp; ++i) {
        uint32_t p = primes[i];
        if (tmp % p == 0) {
            uint64_t exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            tau = tau * ((seq_t)exp + 1);
        }
    }
    if (tmp > 1)                 /* remaining prime factor > sqrt(n) */
        tau = tau * 2;

    return tau;
}

/* --------------------------- main sequence ---------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;         /* safety guard, sequence offset is 1 */

    /* Lazy initialization of cache */
    if (cache_cap == 0) {
        cache_cap = 64;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
    }

    /* Keep track of the next candidate k to test */
    static uint64_t current_k = 1;

    while (cache_len < (size_t)n) {
        /* Ensure we can factorize current_k */
        grow_primes_to(current_k);

        seq_t tau = tau_of_uint64(current_k);
        seq_t g   = gcd((seq_t)current_k, tau);

        if (g == 1) {
            /* cache is full, grow it */
            if (cache_len >= cache_cap) {
                cache_cap *= 2;
                cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
                if (!cache) abort();
            }
            cache[cache_len++] = (seq_t)current_k;
        }
        ++current_k;
    }

    return cache[n - 1];
}
