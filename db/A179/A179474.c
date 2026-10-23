/*
 * A179474  a(n) = position of A045392(n) in A042997.
 * https://oeis.org/A179474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = position (1‑based) of the nth prime ≡2 (mod 7) among primes not ≡1 or 0 (mod 7). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t *primes = NULL;          /* primes found so far */
static size_t primes_len = 0;            /* number of primes stored */
static size_t primes_cap = 0;            /* allocated capacity of primes */

static seq_t *cache_a = NULL;            /* computed a(n) values */
static size_t cache_len = 0;             /* number of terms stored */
static size_t cache_cap = 0;             /* allocated capacity of cache */

static idx_t pos_in_set = 0;             /* count of primes not ≡1 or 0 (mod 7) */
static uint64_t next_candidate = 2;      /* next number to test for primality */

/* Ensure the prime array has room for at least 'needed' primes. */
static inline void ensure_prime_cap(size_t needed)
{
    if (needed <= primes_cap)
        return;
    size_t new_cap = primes_cap > 0 ? primes_cap * 2 : 8;
    while (new_cap < needed)
        new_cap *= 2;
    uint64_t *new_arr = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
    primes = new_arr;
    primes_cap = new_cap;
}

/* Ensure the cache array has room for at least 'needed' terms. */
static inline void ensure_cache_cap(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap > 0 ? cache_cap * 2 : 8;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *new_arr = (seq_t *)realloc(cache_a, new_cap * sizeof(seq_t));
    cache_a = new_arr;
    cache_cap = new_cap;
}

/* Simple primality test using previously found primes. */
static bool is_prime(uint64_t n)
{
    if (n < 2)
        return false;
    for (size_t i = 0; i < primes_len; ++i) {
        uint64_t p = primes[i];
        if (p * p > n)
            break;
        if (n % p == 0)
            return false;
    }
    return true;
}

/* Generate primes and fill cache until we have at least 'target' terms. */
static void generate_up_to(idx_t target)
{
    while (cache_len < (size_t)target) {
        uint64_t p = next_candidate;

        /* Advance to next candidate (skip even numbers after 2). */
        if (next_candidate == 2)
            next_candidate = 3;
        else
            next_candidate += 2;

        if (!is_prime(p))
            continue;

        /* store new prime */
        ensure_prime_cap(primes_len + 1);
        primes[primes_len++] = p;

        /* Update position counter if prime is not ≡1 or 0 (mod 7). */
        if (p % 7 != 1 && p % 7 != 0) {
            pos_in_set++;
            if (p % 7 == 2) {
                ensure_cache_cap(cache_len + 1);
                cache_a[cache_len++] = (seq_t)pos_in_set;
            }
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for n=0, return 0 */
    generate_up_to(n);
    return cache_a[(size_t)n - 1];
}
