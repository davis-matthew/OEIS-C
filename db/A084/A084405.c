/*
 * A084405  Primes whose sum of factorials of digits is also prime.
 * https://oeis.org/A084405
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 415, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A084405:
   a(n) = the n-th prime p such that the sum of factorials of
   its decimal digits is also prime. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"

/* factorials 0! .. 9! */
static const uint64_t fact[10] = {
    1ULL, 1ULL, 2ULL, 6ULL, 24ULL, 120ULL, 720ULL,
    5040ULL, 40320ULL, 362880ULL
};

/* dynamic cache of qualifying primes (stored as uint64_t) */
static idx_t *prime_cache = NULL;     /* array of uint64_t */
static size_t  prime_len  = 0;        /* number of elements cached */
static size_t  prime_cap  = 0;        /* allocated capacity */

/* sieve for primality testing of uint64_t */
static bool   *sieve          = NULL; /* true = composite */
static idx_t   sieve_limit    = 0;    /* current sieve upper bound (inclusive) */
static idx_t   next_candidate = 2;    /* next number to examine */

/* ensure prime_cache has at least 'needed' capacity */
static void ensure_prime_capacity(size_t needed)
{
    if (prime_cap >= needed)
        return;
    size_t new_cap = prime_cap ? prime_cap * 2 : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    idx_t *tmp = (idx_t*)realloc(prime_cache, new_cap * sizeof(idx_t));
    if (!tmp)
        exit(1);
    prime_cache = tmp;
    prime_cap   = new_cap;
}

/* build a fresh sieve up to 'new_limit' (inclusive) */
static void build_sieve(idx_t new_limit)
{
    free(sieve);
    sieve = (bool*)calloc((size_t)new_limit + 1, sizeof(bool));
    if (!sieve)
        exit(1);
    for (idx_t i = 2; i * i <= new_limit; ++i) {
        if (!sieve[i]) {
            for (idx_t j = i * i; j <= new_limit; j += i)
                sieve[j] = true;
        }
    }
    sieve_limit = new_limit;
}

/* add a qualifying prime to the cache */
static void add_prime(idx_t val)
{
    ensure_prime_capacity(prime_len + 1);
    prime_cache[prime_len++] = val;
}

/* scan primes from next_candidate up to sieve_limit, testing the
   factorial‑sum primality condition */
static void scan_for_primes(void)
{
    for (idx_t p = next_candidate; p <= sieve_limit; ++p) {
        if (!sieve[p]) {                    /* p is prime */
            /* compute sum of factorials of decimal digits of p */
            idx_t t = p, sum = 0;
            do {
                sum += fact[t % 10];
                t /= 10;
            } while (t > 0);

            if (is_prime((idx_t)sum))      /* sum is prime? */
                add_prime(p);
        }
        next_candidate = p + 1;
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                    /* undefined for n=0 */

    /* generate terms until we have at least n entries */
    while (prime_len < (size_t)n) {
        /* set up or enlarge sieve as necessary */
        if (sieve_limit == 0) {
            build_sieve(1000000ULL);            /* start with 1 M */
        } else if (next_candidate > sieve_limit) {
            idx_t new_limit = sieve_limit * 2;
            build_sieve(new_limit);
        }
        scan_for_primes();
    }

    return (seq_t)prime_cache[n - 1];
}
