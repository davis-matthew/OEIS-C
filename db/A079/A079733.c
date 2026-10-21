/*
 * A079733  Primes of the form x^2 + y^2 + 3 (x,y nonnegative).
 * https://oeis.org/A079733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes primes of the form x^2 + y^2 + 3 for non‑negative integers x, y. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ *
 *  Static state for primes and cached sequence values
 */
static bool           initialized   = false;
static idx_t        *prime_list     = NULL;
static size_t        prime_list_sz  = 0;
static size_t        prime_list_cap = 0;
static idx_t         sieve_limit    = 2;

static seq_t        *cache          = NULL;
static size_t        cache_sz       = 0;
static size_t        cache_cap      = 0;
static size_t        prime_index    = 0;

/* ------------------------------------------------------------------ *
 *  Add a prime to the list, reallocating if necessary
 */
static void add_prime(idx_t p)
{
    if (prime_list_sz == prime_list_cap) {
        prime_list_cap = prime_list_cap ? prime_list_cap * 2 : 8;
        prime_list = (idx_t *)realloc(prime_list,
                                      prime_list_cap * sizeof(idx_t));
    }
    prime_list[prime_list_sz++] = p;
}

/* ------------------------------------------------------------------ *
 *  Extend the prime list up to new_limit (inclusive).  Uses trial
 *  division against the current list of primes.
 */
static void extend_primes(idx_t new_limit)
{
    if (new_limit <= sieve_limit)
        return;

    for (idx_t cand = sieve_limit + 1; cand <= new_limit; ++cand) {
        bool is_prime = true;
        for (size_t j = 0; j < prime_list_sz; ++j) {
            idx_t p = prime_list[j];
            if ((idx_t)p * (idx_t)p > cand)
                break;
            if (cand % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime)
            add_prime(cand);
    }
    sieve_limit = new_limit;
}

/* ------------------------------------------------------------------ *
 *  Check whether a number n can be expressed as a sum of two squares.
 *  Uses the classical criterion: a number is a sum of two squares iff
 *  in its prime factorisation every prime ≡ 3 (mod 4) has an even
 *  exponent.
 */
static bool is_sum_of_two_squares(uint64_t n)
{
    if (n == 0 || n == 1)
        return true;

    uint64_t m = n;

    /* Ensure we have enough primes for trial division up to sqrt(m) */
    idx_t required = (idx_t)((double)m * 0.5 + 1.0);   /* rough upper bound */
    if (prime_list_sz == 0 || prime_list[prime_list_sz - 1] < required)
        extend_primes(required);

    for (size_t i = 0; i < prime_list_sz; ++i) {
        idx_t p = prime_list[i];
        if ((uint64_t)p * (uint64_t)p > m)
            break;
        if (m % p == 0) {
            int exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            if ((p & 3ULL) == 3ULL && (exp & 1) == 1)
                return false;
        }
    }
    if (m > 1) {                       /* m is a prime > sqrt(original n) */
        if ((m & 3ULL) == 3ULL)
            return false;
    }
    return true;
}

/* ------------------------------------------------------------------ *
 *  Ensure that at least 'need' terms of the sequence are cached.
 */
static void ensure_cache(idx_t need)
{
    if (!initialized) {
        initialized = true;

        /* start the prime list with 2 */
        prime_list_cap = 8;
        prime_list = (idx_t *)malloc(prime_list_cap * sizeof(idx_t));
        add_prime(2);
        sieve_limit = 2;

        /* initialise the cache */
        cache_cap = 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_sz  = 0;
        prime_index = 0;
    }

    while (cache_sz < need) {
        /* generate more primes if we ran out of them */
        if (prime_index == prime_list_sz) {
            idx_t new_limit = sieve_limit * 2 + 100;
            extend_primes(new_limit);
            if (prime_index == prime_list_sz) /* guard against endless loop */
                break;
        }

        idx_t p = prime_list[prime_index++];
        if (p >= 3 && is_sum_of_two_squares((uint64_t)p - 3U)) {
            if (cache_sz == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 8;
                cache = (seq_t *)realloc(cache,
                                         cache_cap * sizeof(seq_t));
            }
            cache[cache_sz++] = (seq_t)p;
        }
    }
}

/* ------------------------------------------------------------------ *
 *  Public sequence function.  a(1) is the first prime of the form
 *  x^2 + y^2 + 3, a(2) the second, etc.  For n < 1 the function
 *  returns 0, but that case is never used in the OEIS tests.
 */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          // undefined for index 0

    ensure_cache(n);
    return cache[n - 1];
}
