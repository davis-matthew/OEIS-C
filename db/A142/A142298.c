/*
 * A142298  Primes congruent to 15 mod 44.
 * https://oeis.org/A142298
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 326, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A142298: primes congruent to 15 (mod 44). 
 * The function generates primes of the form 44·k + 15 using trial division
 * with a precomputed list of small primes up to 2000.  The generated primes
 * are cached statically so calls may arrive in any order. 
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* static cache of the sequence values */
static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;

/* pre‑computed list of primes up to 2000 (there are 303 such primes) */
static uint64_t small_primes[350];
static size_t small_prime_count = 0;
static bool sieve_built = false;

/* build the list of small primes once */
static void build_small_primes(void)
{
    if (sieve_built) return;
    const int limit = 2000;
    bool *sieve = (bool *)malloc(limit + 1);
    if (!sieve) return;                     /* allocation failure – unlikely */

    for (int i = 0; i <= limit; ++i) sieve[i] = true;
    sieve[0] = sieve[1] = false;

    for (int i = 2; i * i <= limit; ++i) {
        if (sieve[i]) {
            for (int j = i * i; j <= limit; j += i) sieve[j] = false;
        }
    }

    for (int i = 2; i <= limit; ++i) {
        if (sieve[i]) {
            small_primes[small_prime_count++] = (uint64_t)i;
        }
    }
    free(sieve);
    sieve_built = true;
}

/* deterministic primality test for 64‑bit integers */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    if (n % 3 == 0) return n == 3;

    build_small_primes();

    for (size_t i = 0; i < small_prime_count; ++i) {
        uint64_t p = small_primes[i];
        if (p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                              */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* undefined for offset 0 */

    /* initialise cache on first call */
    if (!cache) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
    }

    /* candidate starting at 59 (44·1+15) */
    static uint64_t candidate = 59;

    /* Find enough terms */
    while (cache_len < n) {
        if (is_prime_uint64(candidate)) {
            /* grow cache if necessary */
            if (cache_len >= cache_cap) {
                idx_t new_cap = cache_cap << 1;
                seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
                for (idx_t i = 0; i < cache_len; ++i) new_cache[i] = cache[i];
                free(cache);
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)candidate;
        }
        candidate += 44;
    }

    return cache[n - 1];
}
