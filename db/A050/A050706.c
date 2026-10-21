/*
 * A050706  Composites c that reach a prime after 2 iterations of c-> c+sum of
 *          prime factors of c.
 * https://oeis.org/A050706
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A050706: composites that reach a prime after two iterations
   of c → c + sum of prime factors of c. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

/* ----- global cache and sieve ------------------------------------ */
static uint32_t  *g_spf     = NULL;   /* smallest prime factor (0..limit) */
static uint8_t   *g_isprime = NULL;   /* primality bitmap (0..limit)   */
static uint32_t  *g_primes  = NULL;   /* list of primes                */
static size_t     g_prime_cnt = 0;     /* number of primes             */
static size_t     g_limit     = 0;     /* current sieve limit           */

static seq_t    *g_cache    = NULL;
static size_t     g_cache_len = 0;  /* number of stored terms */
static size_t     g_cache_cap = 0;  /* allocated capacity    */

static idx_t     g_next_candidate = 4; /* first composite > 2 */

/* ----- sieve build ----------------------------------------------- */
static void
ensure_sieve(size_t new_limit)
{
    if (new_limit <= g_limit) return;

    /* free previous tables if any */
    free(g_spf);
    free(g_isprime);
    free(g_primes);

    g_spf     = (uint32_t*)malloc((new_limit + 1) * sizeof(uint32_t));
    g_isprime = (uint8_t*)calloc((new_limit + 1), sizeof(uint8_t));
    g_primes  = (uint32_t*)malloc((new_limit + 1) * sizeof(uint32_t));
    if (!g_spf || !g_isprime || !g_primes) abort(); /* out of memory */

    /* linear sieve ------------------------------------------------- */
    g_prime_cnt = 0;
    for (uint32_t i = 2; i <= new_limit; ++i) {
        if (g_spf[i] == 0) {
            g_spf[i] = i;
            g_isprime[i] = 1;
            g_primes[g_prime_cnt++] = i;
        }
        for (size_t j = 0; j < g_prime_cnt &&
                g_primes[j] <= g_spf[i] &&
                (uint64_t)i * g_primes[j] <= new_limit; ++j) {
            g_spf[i * g_primes[j]] = g_primes[j];
        }
    }
    g_limit = new_limit;
}

/* ----- sum of prime factors for numbers ≤ g_limit ---------------- */
static seq_t
sum_prime_factors_small(uint32_t n)
{
    seq_t sum = 0;
    while (n > 1) {
        uint32_t p = g_spf[n];
        sum += (seq_t)p;
        n /= p;
    }
    return sum;
}

/* ----- sum of prime factors for numbers > g_limit ---------------- */
static seq_t
sum_prime_factors_big(uint64_t n)
{
    seq_t sum = 0;
    uint64_t temp = n;
    for (size_t i = 0; i < g_prime_cnt; ++i) {
        uint32_t p = g_primes[i];
        if ((uint64_t)p * (uint64_t)p > temp) break;
        while (temp % p == 0) {
            sum += p;
            temp /= p;
        }
    }
    if (temp > 1) sum += temp;
    return sum;
}

/* ----- cache helper ---------------------------------------------- */
static void
push_cache(seq_t val)
{
    if (g_cache_len >= g_cache_cap) {
        size_t new_cap = g_cache_cap ? g_cache_cap * 2 : 16;
        seq_t *new_arr = (seq_t*)realloc(g_cache, new_cap * sizeof(seq_t));
        if (!new_arr) abort(); /* out of memory */
        g_cache = new_arr;
        g_cache_cap = new_cap;
    }
    g_cache[g_cache_len++] = val;
}

/* ----- main function ---------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety for off‑by‑one callers */

    /* initialise sieve if needed */
    if (g_limit == 0) ensure_sieve(1 << 20);   /* start with 1 048 576 */

    /* extend cache until we have the nth value */
    while (g_cache_len < n) {
        /* find next composite candidate */
        while (true) {
            if (g_next_candidate > g_limit)
                ensure_sieve((size_t)g_next_candidate);

            if (g_next_candidate >= 4 &&
                !g_isprime[(uint32_t)g_next_candidate])
                break;                /* it is composite */
            g_next_candidate++;
        }
        uint32_t cand = g_next_candidate;

        /* first iteration */
        seq_t sum1 = (cand <= g_limit) ?
                     sum_prime_factors_small(cand) :
                     sum_prime_factors_big((uint64_t)cand);
        seq_t c1 = (seq_t)cand + sum1;

        /* ensure we can factor c1 if it is not too large */
        if (c1 > g_limit) ensure_sieve((size_t)c1);

        /* second iteration */
        seq_t sum2 = (c1 <= g_limit) ?
                     sum_prime_factors_small((uint32_t)c1) :
                     sum_prime_factors_big((uint64_t)c1);
        seq_t c2 = c1 + sum2;

        /* prime check on c2 */
        bool isPrimeC2;
        if (c2 <= g_limit) {
            isPrimeC2 = g_isprime[(uint32_t)c2];
        } else {
            isPrimeC2 = is_prime_big(c2);
        }

        if (isPrimeC2) push_cache((seq_t)cand);

        g_next_candidate++;
    }

    return g_cache[n - 1];
}
