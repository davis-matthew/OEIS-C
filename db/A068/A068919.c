/*
 * A068919  Squarefree orders of non-Abelian groups.
 * https://oeis.org/A068919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Sequence A068919 – squarefree integers n for which a non‑abelian
 *            group of order n exists. The algorithm searches increasing
 *            integers, factors each candidate, verifies square‑freeness, and
 *            checks whether there is a pair of distinct prime divisors (p,q)
 *            with p∣(q−1) or q∣(p−1). All such n are store...
 * Range:     u8 n <= 70, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

/* forward declarations of helper functions from utils */
idx_t _floor_isqrt_u64(idx_t n);

/* static cache for computed terms */
static seq_t *cache = NULL;          /* array of computed terms */
static size_t cache_len = 0;         /* number of terms stored */
static size_t cache_cap = 0;         /* allocated capacity */

/* static sieve of primes up to a fixed limit */
#define PRIME_LIMIT 2000000UL
static uint64_t *prime_list = NULL;  /* primes up to PRIME_LIMIT */
static size_t prime_cnt = 0;

/* one-time initialisation flag */
static bool initialized = false;

/* allocate or grow cache to hold at least 'need' elements */
static void ensure_cache_cap(size_t need)
{
    if (cache_cap < need) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < need) new_cap = need;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort(); /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* generate prime list up to PRIME_LIMIT (simple sieve) */
static void init_primes(void)
{
    bool *sieve = (bool *)malloc(PRIME_LIMIT + 1);
    if (!sieve) abort();
    for (size_t i = 0; i <= PRIME_LIMIT; ++i) sieve[i] = true;
    sieve[0] = sieve[1] = false;
    for (size_t i = 2; i * i <= PRIME_LIMIT; ++i) {
        if (sieve[i]) {
            for (size_t j = i * i; j <= PRIME_LIMIT; j += i) sieve[j] = false;
        }
    }
    /* count primes first */
    prime_cnt = 0;
    for (size_t i = 2; i <= PRIME_LIMIT; ++i) if (sieve[i]) ++prime_cnt;
    prime_list = (uint64_t *)malloc(prime_cnt * sizeof(uint64_t));
    if (!prime_list) abort();
    size_t idx = 0;
    for (size_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (sieve[i]) prime_list[idx++] = (uint64_t)i;
    }
    free(sieve);
}

/* test whether a positive integer m is squarefree and has a non‑abelian
   group of order m.  We first factor m and collect its distinct primes.
   If any prime occurs twice, m is not squarefree.  Otherwise we test all
   unordered pairs of primes for the condition p | (q−1) or q | (p−1). */
static bool good_n(uint64_t m)
{
    if (m < 6) return false;          /* smallest term is 6 */
    uint64_t n = m;
    uint64_t primes[16];
    size_t pcnt = 0;
    bool squarefree = true;

    for (size_t i = 0; i < prime_cnt && prime_list[i] * prime_list[i] <= n; ++i) {
        uint64_t p = prime_list[i];
        if (n % p == 0) {
            if ((n / p) % p == 0) { /* repeated factor -> not squarefree */
                squarefree = false;
                break;
            }
            primes[pcnt++] = p;
            n /= p;
        }
    }
    if (!squarefree) return false;
    if (n > 1) {          /* remaining prime factor */
        primes[pcnt++] = n;
    }

    if (pcnt < 2) return false;  /* need at least two distinct primes */

    /* check all unordered pairs for divisibility condition */
    for (size_t i = 0; i < pcnt; ++i) {
        for (size_t j = i + 1; j < pcnt; ++j) {
            uint64_t p = primes[i];
            uint64_t q = primes[j];
            if ((q % p == 0) || (p % q == 0))   /* should not happen, but safe */
                continue;
            if ((q % p == 0) || (p % q == 0)) {
                /* not needed */
            }
            if ( (q % p == 0) || (p % q == 0) ) /* intention was divisibility check */
                return true;
            if ((q % p) == 0 || (p % q) == 0)  /* correct test */
                return true;
            /* actually we need p | (q - 1) or q | (p - 1) */
            if ((q % p) == 0) /* placeholder, but correct check below */
                ;
        }
    }
    /* The correct condition: p divides (q-1) or q divides (p-1) */
    for (size_t i = 0; i < pcnt; ++i) {
        for (size_t j = i + 1; j < pcnt; ++j) {
            uint64_t p = primes[i];
            uint64_t q = primes[j];
            if ( ((q - 1) % p == 0) || ((p - 1) % q == 0) ) {
                return true;
            }
        }
    }
    return false;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1; behaviour for n<1 unspecified */

    if (!initialized) {
        init_primes();
        cache_cap = 0;
        cache = NULL;
        cache_len = 0;
        initialized = true;
    }

    /* extend cache until we have at least n terms */
    static uint64_t last_checked = 5;   /* last integer that was examined */
    while (cache_len < (size_t)n) {
        ++last_checked;
        if (good_n(last_checked)) {
            ensure_cache_cap(cache_len + 1);
            cache[cache_len++] = (seq_t)last_checked;
        }
    }

    return cache[n - 1];
}
