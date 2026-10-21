/*
 * A228649  Numbers n such that n-1, n and n+1 are all squarefree.
 * https://oeis.org/A228649
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for n such that n‑1, n and n+1 are all squarefree
   (OEIS A228649).  The function is memoised so that calls in any
   order return the correct value. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Prime sieve – enough for candidates up to about 10^10
   (squareroot ≈ 1e5). */
#define PRIME_SIEVE_LIMIT 100000
static uint32_t primes[PRIME_SIEVE_LIMIT];
static size_t prime_cnt = 0;
static bool primes_prepared = false;

static void prepare_primes(void)
{
    if (primes_prepared) return;
    bool sieve[PRIME_SIEVE_LIMIT + 1];
    for (size_t i = 0; i <= PRIME_SIEVE_LIMIT; ++i) sieve[i] = true;
    sieve[0] = sieve[1] = false;
    for (size_t i = 2; i * i <= PRIME_SIEVE_LIMIT; ++i)
        if (sieve[i])
            for (size_t j = i * i; j <= PRIME_SIEVE_LIMIT; j += i)
                sieve[j] = false;
    for (size_t i = 2; i <= PRIME_SIEVE_LIMIT; ++i)
        if (sieve[i]) primes[prime_cnt++] = (uint32_t)i;
    primes_prepared = true;
}

/* Test whether a 64‑bit integer is squarefree –
   returns true if no prime square divides the number. */
static bool is_squarefree_uint64(uint64_t x)
{
    if (x < 2) return true;      /* 0,1 are squarefree by definition */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        if (p > x / p) break;   /* p*p > x */
        if (x % p == 0) {
            x /= p;
            if (x % p == 0) return false;
        }
    }
    return true;
}

/* Cache for already computed terms.  The cache holds seq_t values
   and is grown as needed. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

/* Increase the capacity of the cache array. */
static void grow_cache(void)
{
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* Allocation error */
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the n‑th term of the sequence (n ≥ 1).  The function
   works on arbitrarily ordered calls. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* Undefined by the offset, return 0 safely */

    prepare_primes();

    /* Generate new terms until we have at least n terms cached. */
    uint64_t candidate = (cache_len > 0) ? (uint64_t)cache[cache_len - 1] + 1 : 2;
    while (cache_len < n) {
        if (is_squarefree_uint64(candidate - 1) &&
            is_squarefree_uint64(candidate)     &&
            is_squarefree_uint64(candidate + 1)) {
            if (cache_len == cache_cap) grow_cache();
            cache[cache_len++] = (seq_t)candidate;
        }
        ++candidate;
    }

    return cache[n - 1];
}
