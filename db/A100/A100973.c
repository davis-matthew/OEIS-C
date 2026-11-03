/*
 * A100973  Integers that are Rhonda numbers to base 9.
 * https://oeis.org/A100973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 9, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 52 terms through n = 885 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for Rhonda numbers to base 9 */
/* The function computes the nth Rhonda number to base 9. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Prime sieve up to 1 000 000 for trial division factorisation.     */
/* ------------------------------------------------------------------ */
#define PRIME_LIMIT 1000000
static bool is_composite[PRIME_LIMIT + 1];
static uint32_t primes[78498];          /* number of primes ≤ 1 000 000 */
static uint32_t prime_count = 0;
static bool primes_initialized = false;

static void init_prime_sieve(void) {
    if (primes_initialized) return;
    primes_initialized = true;
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!is_composite[i]) {
            primes[prime_count++] = i;
            if ((uint64_t)i * i <= PRIME_LIMIT) {
                for (uint32_t j = i * i; j <= PRIME_LIMIT; j += i) {
                    is_composite[j] = true;
                }
            }
        }
    }
}

/* ------------------------------------------------------------------ */
/* Helper: test whether a positive integer is a Rhonda number to base 9 */
/* ------------------------------------------------------------------ */
/* in 64 bits (in seq_t this was too slow under big); the digit product
   must be 9 times the sum, so a multiple of 9 */
static bool is_rhonda(uint64_t n) {
    uint64_t prod = 1;
    uint64_t tmp = n;
    while (tmp > 0) {
        uint64_t d = tmp % 9;
        if (d == 0) return false;
        prod = prod * d;
        tmp = tmp / 9;
    }
    if (prod % 9) return false;
    uint64_t sum = 0;
    uint64_t m = n;
    for (uint32_t i = 0; i < prime_count; ++i) {
        uint64_t p = primes[i];
        if (p * p > m) break;
        while (m % p == 0) {
            sum += p;
            m = m / p;
        }
    }
    if (m > 1) sum += m;                /* remaining prime factor */
    return prod == 9 * sum;
}

/* ------------------------------------------------------------------ */
/* Cache for already found Rhonda numbers                               */
/* ------------------------------------------------------------------ */
static idx_t current = 15539;           /* will be incremented to 15540 on first loop */

#ifdef __cplusplus
#include <vector>
static std::vector<seq_t> cache;
#else
static seq_t *cache = NULL;
static idx_t cache_size = 0;
static idx_t cache_capacity = 0;
#endif

/* ------------------------------------------------------------------ */
/* Sequence function                                                   */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n) {
    if (n == 0) return 0;               /* undefined: return 0 as placeholder */

    init_prime_sieve();

#ifdef __cplusplus
    while ((idx_t)cache.size() < n) {
        ++current;
        if (is_rhonda((uint64_t)current))
            cache.push_back((seq_t)current);
    }
    return cache[n - 1];
#else
    while (cache_size < n) {
        ++current;
        if (is_rhonda((uint64_t)current)) {
            if (cache_size == cache_capacity) {
                idx_t new_cap = cache_capacity ? cache_capacity * 2 : 64;
                seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_arr) exit(1);  /* allocation failure */
                cache = new_arr;
                cache_capacity = new_cap;
            }
            cache[cache_size++] = (seq_t)current;
        }
    }
    return cache[n - 1];
#endif
}
