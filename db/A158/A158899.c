/*
 * A158899  These are numbers n such that the reciprocal, 1/n, is a repeating
 *          fraction whose period is n/2 - 1.
 * https://oeis.org/A158899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A158899: numbers 2p where p is a full‑reptend prime (period of 1/p is p−1).  */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mod_pow.h"

/* ---------------------------------------------------------------------- */
/* Simple prime sieve – rebuilt when more primes are needed.              */
static uint32_t *primes = NULL;      /* array of primes up to current limit */
static size_t primes_len = 0;        /* number of primes currently stored   */
static size_t primes_cap = 0;        /* current allocation capacity         */
static uint64_t prime_limit = 0;     /* maximum prime value stored          */

/* Rebuild the prime table up to at least 'limit'.  The operation is cheap
   because it is only triggered when we need more primes. */
static void recompute_primes(uint64_t limit)
{
    if (prime_limit >= limit) return;
    free(primes);
    primes = NULL;
    primes_len = 0;
    primes_cap = 0;
    prime_limit = limit;

    bool *is_comp = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!is_comp) exit(EXIT_FAILURE);
    for (uint64_t i = 0; i <= limit; ++i) is_comp[i] = false;

    for (uint64_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            if (primes_len == primes_cap) {
                size_t newcap = primes_cap ? primes_cap * 2 : 16;
                uint32_t *tmp = (uint32_t *)realloc(primes, newcap * sizeof(uint32_t));
                if (!tmp) { free(is_comp); free(primes); exit(EXIT_FAILURE); }
                primes = tmp;
                primes_cap = newcap;
            }
            primes[primes_len++] = (uint32_t)i;
            if (i * i <= limit) {
                for (uint64_t j = i * i; j <= limit; j += i) is_comp[j] = true;
            }
        }
    }
    free(is_comp);
}

/* ---------------------------------------------------------------------- */
/* Factorization of n to distinct prime factors.                          */
static void factor_unique(uint64_t n, uint64_t *fac, size_t *cnt)
{
    *cnt = 0;
    for (size_t i = 0; i < primes_len && (uint64_t)primes[i] * primes[i] <= n; ++i) {
        uint64_t p = primes[i];
        if (n % p == 0) {
            fac[(*cnt)++] = p;
            while (n % p == 0) n /= p;
        }
    }
    if (n > 1) fac[(*cnt)++] = n;
}

/* ---------------------------------------------------------------------- */
/* Test whether p is a full‑reptend prime (for base 10).                  */
static bool is_full_reptend_prime(uint64_t p)
{
    /* 2 and 5 never give a full period in base 10. */
    if (p == 2 || p == 5) return false;

    uint64_t m = p - 1;
    uint64_t order = m;
    uint64_t fac[64];
    size_t fc;
    factor_unique(m, fac, &fc);

    for (size_t i = 0; i < fc; ++i) {
        uint64_t q = fac[i];
        while (order % q == 0 &&
               mod_pow((seq_t)10, order / q, (seq_t)p) == (seq_t)1) {
            order /= q;
        }
    }
    return order == m;
}

/* ---------------------------------------------------------------------- */
/* Cache for computed sequence terms.                                     */
static seq_t *cache   = NULL;   /* stored a(n) values */
static size_t cache_len = 0;    /* number of stored terms  */
static size_t cache_cap = 0;    /* allocation capacity     */
static size_t prime_idx = 0;    /* next prime index to test */

/* ---------------------------------------------------------------------- */
/* Main sequence function.                                                */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* below offset; harmless */

    /* Generate terms until we have at least n of them. */
    while (cache_len < n) {
        /* Ensure the prime list is long enough. */
        if (primes_len == 0 || prime_idx >= primes_len) {
            uint64_t new_limit = prime_limit ? prime_limit * 2 : 1000;
            if (new_limit <= prime_limit) new_limit = prime_limit + 1000;
            recompute_primes(new_limit);
            if (prime_idx >= primes_len) prime_idx = primes_len - 1;
            continue;
        }

        uint64_t p = primes[prime_idx++];
        if (p <= 5) continue;               /* skip 2 and 5 */
        if (is_full_reptend_prime(p)) {
            /* Append 2*p to cache */
            if (cache_len == cache_cap) {
                size_t newcap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                if (!tmp) { free(cache); exit(EXIT_FAILURE); }
                cache = tmp;
                cache_cap = newcap;
            }
            cache[cache_len++] = (seq_t)2 * (seq_t)p;
        }
    }
    return cache[n - 1];   /* offset is 1, so a(1) is cache[0] */
}
