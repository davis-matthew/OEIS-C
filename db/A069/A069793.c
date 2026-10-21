/*
 * A069793  Find smallest k such that prime(k) and prime(n+k) use the same
 *          digits; sequence gives prime(k).
 * https://oeis.org/A069793
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   same_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A069793: for n ≥ 0, a(n) is the smallest prime p_k
   such that p_k and p_{k+n} are anagrams (use the same decimal digits). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <math.h>
#include "../utils/same_digits.h"

/* Prime generation ---------------------------------------------------- */
/* Prime limit big enough for the required queries (n ≤ 10000). */
static const uint32_t PRIME_LIMIT = 20000000;   /* ≈ 1.27 M primes */

/* 1‑based array of primes, prime_array[0] unused.              */
static uint64_t *prime_array = NULL;
static size_t    prime_count = 0;

/* Simple Eratosthenes sieve, called lazily. */
static void build_sieve(void)
{
    if (prime_array != NULL)
        return;                     /* already built */

    /* Allocate maximum size for primes in this range */
    prime_count = PRIME_LIMIT / (log((double)PRIME_LIMIT) - 1.0) + 100; /* rough bound */
    prime_array = (uint64_t *)calloc(prime_count + 1, sizeof(uint64_t));
    if (!prime_array) abort();

    uint8_t *sieve = (uint8_t *)calloc(PRIME_LIMIT + 1, sizeof(uint8_t));
    if (!sieve) abort();

    size_t idx = 0;
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!sieve[i]) {
            ++idx;
            prime_array[idx] = i;
            if (idx >= prime_count)  /* safeguard */
                break;
            if ((uint64_t)i * i <= PRIME_LIMIT) {
                for (uint32_t j = i * i; j <= PRIME_LIMIT; j += i)
                    sieve[j] = 1;
            }
        }
    }
    prime_count = idx;
    free(sieve);
}

/* Return the k‑th prime (1‑based).  If k is too large, abort. */
static inline uint64_t get_prime(idx_t k)
{
    if (k == 0) return 0;     /* unused */
    if (k > prime_count) abort();   /* should never happen with the chosen limit */
    return prime_array[k];
}

/* -------------------------------------------------------------- */

/* -------------------------------------------------------------- */
/* Cache for a(n).  0 indicates “not yet computed”. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;

static void ensure_cache(size_t needed)
{
    if (needed <= cache_cap) return;
    seq_t *new_cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
    if (!new_cache) abort();
    /* Initialize new slots to 0. */
    for (size_t i = cache_cap; i < needed; ++i)
        new_cache[i] = 0;
    cache = new_cache;
    cache_cap = needed;
}

/* -------------------------------------------------------------- */
/* The requested sequence function. */
seq_t a(idx_t n)
{
    /* Initialize sieve and cache lazily. */
    if (prime_array == NULL)
        build_sieve();
    if (cache == NULL)
        ensure_cache(1);

    if (n >= cache_cap)
        ensure_cache(n + 1);

    if (cache[n] != 0)                 /* already computed */
        return cache[n];

    /* Search for the smallest k ≥ 1 such that primes[k] and primes[k+n] are anagrams. */
    idx_t k = 1;
    while (true) {
        uint64_t p1 = get_prime(k);
        uint64_t p2 = get_prime(k + n);
        if (same_digits(p1, p2)) {
            cache[n] = (seq_t)p1;
            return cache[n];
        }
        ++k;
    }
}
