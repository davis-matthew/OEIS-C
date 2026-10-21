/*
 * A088380  Numbers not exceeding the cube of their smallest prime factor.
 * https://oeis.org/A088380
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 703, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A088380 – Numbers not exceeding the cube of their smallest prime factor.
   a(1) = 1, a(2) = 2, a(3) = 3, a(4) = 4, ...
   The function computes the nth term (offset 1) using an incremental
   linear sieve for the smallest prime factor. */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache   = NULL;   /* stored sequence values          */
static size_t cache_len = 0, cache_cap = 0;

static uint32_t *spf   = NULL;   /* array of smallest prime factor */
static idx_t spf_cap   = 0;      /* size of spf array               */

static uint32_t *primes   = NULL; /* list of primes discovered */
static size_t primes_len = 0, primes_cap = 0;

static idx_t current_m = 0;      /* last m that was tested         */

/* Append a new term to the cache, growing the array as necessary. */
static void cache_append(seq_t val) {
    if (cache_len == cache_cap) {
        cache_cap = cache_cap ? cache_cap * 2 : 8;
        seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
    }
    cache[cache_len++] = val;
}

/* Reallocate and rebuild the sieve up to at least 'limit' (inclusive).  */
static void extend_sieve(idx_t limit) {
    if (limit <= spf_cap) return;

    /* Reallocate spf array. */
    uint32_t *new_spf = (uint32_t *)realloc(spf, (size_t)(limit + 1) * sizeof(uint32_t));
    if (!new_spf) exit(1);
    spf = new_spf;

    /* Initialise new entries to 0. */
    for (idx_t i = spf_cap + 1; i <= limit; ++i) spf[i] = 0;
    spf[0] = 0;
    spf[1] = 1;

    /* Linear sieve from 2 up to limit. */
    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {               /* i is prime */
            spf[i] = (uint32_t)i;
            if (primes_len == primes_cap) {
                primes_cap = primes_cap ? primes_cap * 2 : 8;
                uint32_t *tmp = (uint32_t *)realloc(primes, primes_cap * sizeof(uint32_t));
                if (!tmp) exit(1);
                primes = tmp;
            }
            primes[primes_len++] = (uint32_t)i;
        }
        for (size_t pi = 0; pi < primes_len; ++pi) {
            uint32_t p = primes[pi];
            if ((idx_t)p > spf[i] || (idx_t)i * p > limit) break;
            spf[i * p] = p;
        }
    }

    spf_cap = limit;
}

/* Return the nth term of the sequence (offset 1). */
seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined for offset 1 – harmless */

    while (cache_len < n) {
        /* Ensure the sieve covers the next integer to test. */
        if (current_m + 1 > spf_cap) {
            idx_t new_limit = spf_cap ? spf_cap * 2 : 1024;
            if (new_limit < (current_m + 1) * 2) new_limit = (current_m + 1) * 2;
            extend_sieve(new_limit);
        }

        ++current_m;
        seq_t m = (seq_t)current_m;
        uint32_t p = spf[current_m];
        seq_t cube = (seq_t)p * p * p;   /* p^3 – fits in seq_t */
        if (m <= cube) cache_append(m);
    }

    return cache[n - 1];
}
