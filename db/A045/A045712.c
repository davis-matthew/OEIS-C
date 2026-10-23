/*
 * A045712  Primes with first digit 6.
 * https://oeis.org/A045712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 620, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   first_digit_is_6
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes the n‑th prime whose first decimal digit is 6
    (OEIS A045712).  The function searches primes incrementally
    and keeps a cache of those starting with 6, so subsequent
    calls reuse the work. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/first_digit_is_6.h"

/* --- helpers ----------------------------------------------------- */

/* --- static state ------------------------------------------------ */
static uint64_t *prime_list   = NULL;   /* primes found so far          */
static size_t   prime_size   = 0;       /* number of primes stored      */
static size_t   prime_cap    = 0;       /* capacity of prime_list       */

static seq_t *cache      = NULL;       /* primes with first digit 6    */
static size_t cache_size = 0;           /* number of cached terms       */
static size_t cache_cap  = 0;           /* capacity of cache array      */

static uint64_t last_checked = 1ULL;    /* last integer examined (prime or not) */

/* ensure that cache has at least n elements */
static void ensure_n(idx_t n)
{
    if (prime_size == 0) {
        /* initialise prime list */
        prime_cap    = 64;
        prime_list   = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
        prime_list[0] = 2ULL;
        prime_size   = 1;
        last_checked = 2ULL;
    }

    /* grow cache array as needed */
    if (cache_cap < n) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 64;
        if (new_cap < (size_t)n) new_cap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;  /* assume realloc succeeded; failure propagates as crash */
        cache_cap = new_cap;
    }

    /* generate primes until we have n cached primes */
    while (cache_size < (size_t)n) {
        uint64_t candidate = last_checked + 1ULL;

        /* primality test: trial division by known primes */
        bool is_prime = true;
        for (size_t i = 0; i < prime_size; ++i) {
            uint64_t p = prime_list[i];
            if (p > candidate / p) break;           /* p*p > candidate */
            if (candidate % p == 0ULL) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            /* add to prime list */
            if (prime_size == prime_cap) {
                size_t new_cap = prime_cap << 1;
                uint64_t *tmp = (uint64_t *)realloc(prime_list,
                                                    new_cap * sizeof(uint64_t));
                prime_list = tmp;
                prime_cap = new_cap;
            }
            prime_list[prime_size++] = candidate;

            /* if leading digit is 6, add to cache */
            if (first_digit_is_6(candidate)) {
                cache[cache_size++] = (seq_t)candidate;
            }
        }

        last_checked = candidate;
    }
}

/* --- public function ---------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0u)   /* OEIS uses 1‑based indexing; ignore n==0 */
        return (seq_t)0;
    ensure_n(n);
    return cache[(size_t)n - 1];
}
