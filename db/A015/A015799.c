/*
 * A015799  phi(n) + 8 | sigma(n).
 * https://oeis.org/A015799
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth integer m such that φ(m)+8 | σ(m)
 * Range:     u8 n <= 4, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External linkage */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

static void ensure_primes_up_to_limit(uint32_t limit);
static void add_prime(uint32_t p);

/* Static state */
static bool initialized = false;
static uint32_t *primes = NULL;
static size_t primes_used = 0, primes_cap = 0;
static uint32_t next_prime_candidate = 2;

static seq_t *cache = NULL;
static size_t cache_len = 0, cache_cap = 0;
static uint64_t next_candidate = 2;

static void init_static_state(void)
{
    if (initialized) return;
    /* Allocate initial space for primes and cache */
    primes_used = 0; primes_cap = 8;
    primes = (uint32_t *)realloc(NULL, primes_cap * sizeof(uint32_t));

    /* Will generate primes incrementally.  No need to pre-fill here. */

    cache_cap = 8;
    cache = (seq_t *)realloc(NULL, cache_cap * sizeof(seq_t));
    cache_len = 0;
    initialized = true;
}

static void add_prime(uint32_t p)
{
    if (primes_used == primes_cap) {
        size_t new_cap = (primes_cap == 0) ? 8 : 2 * primes_cap;
        uint32_t *tmp = (uint32_t *)realloc(primes, new_cap * sizeof(uint32_t));
        primes = tmp;
        primes_cap = new_cap;
    }
    primes[primes_used++] = p;
}

/* Generate all primes ≤ limit, adding them to the primes array. */
static void ensure_primes_up_to_limit(uint32_t limit)
{
    while (next_prime_candidate <= limit) {
        bool is_prime = true;
        for (size_t i = 0; i < primes_used &&
                 (uint64_t)primes[i] * primes[i] <= next_prime_candidate; ++i) {
            if (next_prime_candidate % primes[i] == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime)
            add_prime(next_prime_candidate);
        ++next_prime_candidate;
    }
}

/* Compute the nth element of A015799 (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined, but return 0 for safety */
    init_static_state();

    /* Extend the cache until we have at least n elements. */
    while ((size_t)n > cache_len) {
        uint32_t m = (uint32_t)next_candidate;

        /* Ensure sufficient primes for factorisation. */
        ensure_primes_up_to_limit(m);

        /* Factor m using the precomputed primes. */
        uint32_t temp = m;
        seq_t phi = (seq_t)m;
        seq_t sigma = 1;

        for (size_t i = 0; i < primes_used && (uint64_t)primes[i] * primes[i] <= temp; ++i) {
            uint32_t p = primes[i];
            if (temp % p == 0) {
                int e = 0;
                seq_t term = 1;
                seq_t pe = 1;
                while (temp % p == 0) {
                    temp /= p;
                    ++e;
                    pe *= p;
                    term += pe;
                }
                phi = phi / p * (p - 1);
                sigma *= term;
            }
        }
        if (temp > 1) {          /* remaining prime factor */
            uint32_t p = temp;
            phi = phi / p * (p - 1);
            sigma *= ((seq_t)p + 1);
        }

        if ((sigma % (phi + (seq_t)8)) == 0) {
            /* Store the found integer in the cache. */
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 8 : 2 * cache_cap;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)m;
            /* Proceed to next candidate. */
            next_candidate = m + 1;
            break;
        }
        ++next_candidate;
    }

    return cache[n - 1];
}
